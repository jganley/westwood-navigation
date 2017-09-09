#include "provided.h"
#include <string>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;

// node for A*
struct Node {
    Node()
    {
        parent = nullptr;
        curr = GeoCoord();
        f = g = h = 0;
    }
    Node(Node* parent, GeoCoord curr, double f, double g, double h)
    : parent(parent), curr(curr), f(f), g(g), h(h)
    {}
    Node* parent;
    GeoCoord curr;
    double f, g, h;
};

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    MapLoader map;
    AttractionMapper att;
    SegmentMapper seg;
    
    // helper function for the navigator
    bool doNodesMatch(const Node* lhs, const Node* rhs) const {
        return lhs->curr.latitudeText == rhs->curr.latitudeText && lhs->curr.longitudeText == rhs->curr.longitudeText;
    }
    
    bool doGeoCoordsMatch(const GeoCoord& lhs, const GeoCoord& rhs) const {
        return lhs.latitudeText == rhs.latitudeText && lhs.longitudeText == rhs.longitudeText;
    }
    
    bool doSegmentsMatch(const StreetSegment& lhs, const StreetSegment& rhs) const {
        bool match;
        match = doGeoCoordsMatch(lhs.segment.start, rhs.segment.start) && doGeoCoordsMatch(lhs.segment.end, rhs.segment.end);
        return match;
    }
    
    /*
    void printGeoCoord(const GeoCoord& printMe) const {
        cerr << printMe.latitudeText + ", " + printMe.longitudeText << endl;
    }
    */
    
    // compute the nav segments from path of GeoCoords
    void findFinalPath(const vector<GeoCoord>& path, vector<NavSegment>& nav) const {
        
        /* CONSTRUCTOR OF A NAV SEGMENT PROCEED
         string direction
         string streetName
         double distance
         GeoSegment gs
        */
        
        /* CONSTRUCTOR OF A NAV SEGMENT TURN
         string direction
         string streetName
        */
        
        // current segment
        NavSegment current;
        string direction;
        string streetName;
        double distance;
        GeoSegment gs;
        
        // to determine if turns need to be made
        vector<string> streetNames;
        vector<GeoSegment> streetSegments;
        
        //create a correctly ordered vector
        vector<GeoCoord> correct;
        
        // traverse the vector backwards
        vector<GeoCoord>::const_iterator it = path.end()-1;
        for( ; it != path.begin(); it--) {
            correct.push_back(*it);
        }
        correct.push_back(*it);
        
        // traverse the correct vector
        for(int i = 0; i < correct.size()-1; i++) {
            
            // get the GeoSegment and distance of GeoSegment
            gs = GeoSegment(correct[i],correct[i+1]);
            streetSegments.push_back(gs);
            distance = distanceEarthMiles(correct[i], correct[i+1]);
            
            // get the angle of the GeoSegment and determine cardinal direction
            double angle = angleOfLine(gs);
            if(angle >= 0 && angle <= 22.5)
                direction = "east";
            else if(angle > 0 && angle <= 67.5)
                direction = "northeast";
            else if(angle > 67.5 && angle <= 112.5)
                direction = "north";
            else if(angle > 112.5 && angle <= 157.5)
                direction = "northwest";
            else if(angle > 157.5 && angle <= 202.5)
                direction = "west";
            else if(angle > 202.5 && angle <= 247.5)
                direction = "southwest";
            else if(angle > 247.5 && angle <= 292.5)
                direction = "south";
            else if(angle >= 292.5 && angle <= 337.5)
                direction = "southeast";
            else
                direction = "east";
            
            // find the street name
            vector<StreetSegment> s1;
            s1 = seg.getSegments(correct[i]);
            vector<StreetSegment> s2;
            s2 = seg.getSegments(correct[i+1]);
            
            bool s1Bigger = s1.size() > s2.size();
            for(int j = 0; j < max(s1.size(), s2.size()); j++) {
                
                if(s1Bigger) {
                    
                    for(int k = 0; k < min(s1.size(), s2.size()); k++) {
                        if(doSegmentsMatch(s1[j], s2[k])) {
                            streetName = s1[j].streetName;
                            streetNames.push_back(streetName);
                            break;
                        }
                    }
                    
                } else {
                    
                    for(int k = 0; k < min(s1.size(), s2.size()); k++) {
                        if(doSegmentsMatch(s2[j], s1[k])) {
                            streetName = s2[j].streetName;
                            streetNames.push_back(streetName);
                            break;
                        }
                    }
                    
                }
            }

            // decide if TURN segment is needed
            if(streetNames.size() > 1 && streetNames[streetNames.size()-1] != streetNames[streetNames.size()-2]) {
                
                double leftOrRight = angleBetween2Lines(streetSegments[streetSegments.size()-2], streetSegments[streetSegments.size()-1]);
                
                // create TURN SEGMENT
                if(leftOrRight < 180)
                    current = NavSegment("left", streetNames[streetNames.size()-1]);
                else
                    current = NavSegment("right", streetNames[streetNames.size()-1]);
                nav.push_back(current);
            }
            
            // create PROCEED segment
            current = NavSegment(direction, streetNames[streetNames.size()-1], distance, gs);
            nav.push_back(current);
        }
    }
    
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    // if did not load correctly, return false
    if(!map.load(mapFile))
        return false;
    
    // load the MapLoader data into the
    // attraction mapper and segment mapper
    att.init(map);
    seg.init(map);

	return true;
}

// using the A* algorithm
NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    
    /* KEEP TRACK OF ALL DYNAMICALLY ALLOCATED NODES
    Root
    FINALRESULT
    open
    closed
    */
    
    // CLEAR DIRECTIONS
    directions.clear();
    
    // find GeoCoords of the start and end attractions
    GeoCoord beginCoord;
    if(att.getGeoCoord(start, beginCoord) == false)
        return NAV_BAD_SOURCE;
    GeoCoord endCoordTemp;
    if(att.getGeoCoord(end, endCoordTemp) == false)
        return NAV_BAD_DESTINATION;
    
    /*
    // FOR TESTING PUPOSES
    printGeoCoord(beginCoord);
    printGeoCoord(endCoordTemp);
    cerr << endl;
    */
     
    // get the segment that the attraction is on
    vector<StreetSegment> endSegment;
    endSegment = seg.getSegments(endCoordTemp);
    StreetSegment attractionSegment = endSegment[0];
    
    
    // create an ending Node that will be filled if result is found
    Node* FINALRESULT = nullptr;
    
    
    // create the containers for used and potential nodes
    vector<Node*> open;
    vector<Node*> closed;
    
    
    // add the starting Node
    Node* root = new Node;
    root->parent = nullptr;
    root->curr = beginCoord;
    root->g = 0;
    root->h = distanceEarthMiles(beginCoord, endCoordTemp);
    root->f = root->g + root->h;
    open.push_back(root);

    
    // loop through all the open nodes
    bool areWeDone = false;
    while(!open.empty() && areWeDone == false) {
        
        
        // find lowest value in priority queue and pop it
        vector<Node*>::iterator lowest = open.begin();
        for(int i = 0; i < open.size(); i++) {
            if( open[i]->f < (*lowest)->f )
                lowest = open.begin()+i;
        }
        Node* tryThis = *lowest;
        open.erase(lowest);
        
        
        /*
        // FOR TESTING PURPOSES
        printGeoCoord(tryThis->curr);
        */
        
        
        // put the tryThis node into the closed list
        closed.push_back(tryThis);
        

        // push the succeeding segs
        vector<StreetSegment> testSegments;
        testSegments = seg.getSegments(tryThis->curr);
        
        
        // if one of these segments matches the segment of the attraction, we found it
        for(auto it: testSegments) {
            if(doSegmentsMatch(it, attractionSegment)) {
                areWeDone = true;
                if(FINALRESULT != nullptr)
                    delete FINALRESULT;
                FINALRESULT = tryThis;
                break;
            }
        }
        // break out of while loop if done
        if(areWeDone)
            break;
               
        
        // loop through resulting vectors
        for(auto it: testSegments) {
            
            
            // bools to decide if endpoints should be added to open list
            bool addStart = true;
            bool addEnd = true;
            
            
            // ------- STARTING NODE -------
            
            
            // generatar Node for the start of the segment
            Node* startPart = new Node;
            startPart->parent = tryThis;
            startPart->curr = it.segment.start;
            startPart->g  = tryThis->g + distanceEarthMiles(startPart->curr, tryThis->curr);
            startPart->h = distanceEarthMiles(startPart->curr, endCoordTemp);
            startPart->f = startPart->g + startPart->h;
            
            
            // check if this part matches the original GeoCoord
            if(doNodesMatch(startPart, tryThis)) {
                addStart = false;
            }
            
            
            // check if starting node is in the open list
            vector<Node*>::iterator matchOpen = open.begin();
            for( ; matchOpen != open.end() && addStart; matchOpen++) {
                if(doNodesMatch(*matchOpen, startPart))
                    if((*matchOpen)->f < startPart->f)
                        addStart = false;
            }
            
            
            // check if the node is in the closed list
            vector<Node*>::iterator matchClosed = closed.begin();
            for( ; matchClosed != closed.end() && addStart; matchClosed++) {
                if(doNodesMatch(*matchClosed, startPart))
                    if((*matchClosed)->f < startPart->f)
                        addStart = false;
            }
            
            
            // if possible put the node into the open list
            if(addStart) {
                open.push_back(startPart);
            }
            else {
                delete startPart;
                startPart = nullptr;
            }
            
            
            // ------ ENDPOINT NODE -------
            
            
            // generate Node for the end of the segment
            Node* endPart = new Node;
            endPart->parent = tryThis;
            endPart->curr = it.segment.end;
            endPart->g  = tryThis->g + distanceEarthMiles(endPart->curr, tryThis->curr);
            endPart->h = distanceEarthMiles(endPart->curr, endCoordTemp);
            endPart->f = endPart->g + endPart->h;
            
            
            // check if this part matches the original GeoCoord
            if(doNodesMatch(endPart, tryThis)) {
                addEnd = false;
            }
            
            
            // check if ending node is in the open or closed list
            vector<Node*>::iterator matchOpenEnd = open.begin();
            for( ; matchOpenEnd != open.end() && addEnd; matchOpenEnd++) {
                if(doNodesMatch(*matchOpenEnd, endPart))
                    if((*matchOpenEnd)->f < endPart->f)
                        addEnd = false;
            }
            
            
            // check if ending node is in the closed list
            vector<Node*>::iterator matchClosedEnd = closed.begin();
            for( ; matchClosedEnd != closed.end() && addEnd; matchClosedEnd++) {
                if(doNodesMatch(*matchClosedEnd, endPart))
                    if((*matchClosedEnd)->f < endPart->f)
                        addEnd = false;
            }
            
            
            // if possible put the node into the open list
            if(addEnd) {
                open.push_back(endPart);
            }
            else {
                delete endPart;
                endPart = nullptr;
            }
        }
    }
    
    
    // did not find anything
    if(!areWeDone) {
        return NAV_NO_ROUTE;
    }

    
    vector<GeoCoord> path;
    path.push_back(endCoordTemp);
    //printGeoCoord(endCoordTemp);
    Node findPath = *FINALRESULT;
    while(findPath.parent != nullptr) {
        path.push_back(findPath.curr);
        //printGeoCoord(findPath.curr);
        findPath = *(findPath.parent);
    }
    path.push_back(beginCoord);
    
    findFinalPath(path, directions);
    
    /* delete all dynamic memory
    delete root;
    delete FINALRESULT;
    for(auto it: open)
        if(it != nullptr)
            delete it;
    for(auto it: closed)
        if(it != nullptr)
            delete it;
     */
    
    return NAV_SUCCESS;
}


// operator overloads
bool operator<(const Node& lhs, const Node& rhs) {
    return lhs.f < rhs.f;
}

bool operator==(const Node& lhs, const Node& rhs) {
    return lhs.parent == rhs.parent && lhs.curr.latitudeText == rhs.curr.latitudeText &&
    lhs.curr.longitudeText == rhs.curr.longitudeText && lhs.f == rhs.f &&
    lhs.g == rhs.g && lhs.h == rhs.h;
}

bool operator>(const Node& lhs, const Node& rhs) {
    return lhs.f > rhs.f;
}


//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
