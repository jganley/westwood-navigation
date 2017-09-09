#include "provided.h"
#include "MyMap.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
    
private:
    MyMap<GeoCoord, vector<StreetSegment>> m_geoCoords;
    
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}



// FOR TESTING PURPOSES
bool operator==(const GeoCoord& lhs, const GeoCoord& rhs);



void SegmentMapperImpl::init(const MapLoader& ml)
{
    for(int i = 0; i < ml.getNumSegments(); i++) {
        
        // gets each segment in the map data
        StreetSegment whoDat;
        ml.getSegment(i, whoDat);
        
        // checks to see if start geoCoords are already in the tree
        const vector<StreetSegment>* testCoord = m_geoCoords.find(whoDat.segment.start);
        if(testCoord == nullptr) {
            vector<StreetSegment> startCoord;
            startCoord.push_back(whoDat);
            m_geoCoords.associate(whoDat.segment.start, startCoord);
        }
        // association already exists for that geoCoord
        else {
            // make sure this segment is not already in the vector
            vector<StreetSegment>::const_iterator it = (*testCoord).begin();
            for(; it < (*testCoord).end(); it++) {
                // street segment already exists
                if((*it).streetName == whoDat.streetName &&
                   (*it).segment.start.latitudeText == whoDat.segment.start.latitudeText &&
                   (*it).segment.start.longitudeText == whoDat.segment.start.longitudeText &&
                   (*it).segment.end.latitudeText == whoDat.segment.end.latitudeText &&
                   (*it).segment.start.longitudeText == whoDat.segment.start.longitudeText
                   )
                    break;
            }
            // if nullptr, then the StreetSegment was not found
            if(it == (*testCoord).end()) {
                // associate updated vector with the geoCoord
                vector<StreetSegment> newOne = (*testCoord);
                newOne.push_back(whoDat);
                m_geoCoords.associate(whoDat.segment.start, newOne);
            }
        }
        
        
        // checks to see if ending geoCoords are already in the tree
        const vector<StreetSegment>* testCoordEnd = m_geoCoords.find(whoDat.segment.end);
        if(testCoordEnd == nullptr) {
            vector<StreetSegment> endCoord;
            endCoord.push_back(whoDat);
            m_geoCoords.associate(whoDat.segment.end, endCoord);
        }
        
        // association already exists for that geoCoord
        else {
            // make sure this segment is not already in the vector
            vector<StreetSegment>::const_iterator it = (*testCoordEnd).begin();
            for(; it < (*testCoordEnd).end(); it++) {
                // street segment already exists
                if((*it).streetName == whoDat.streetName &&
                   (*it).segment.start.latitudeText == whoDat.segment.start.latitudeText &&
                   (*it).segment.start.longitudeText == whoDat.segment.start.longitudeText &&
                   (*it).segment.end.latitudeText == whoDat.segment.end.latitudeText &&
                   (*it).segment.start.longitudeText == whoDat.segment.start.longitudeText
                   )
                    break;
            }
            // if nullptr, then the StreetSegment was not found
            if(it == (*testCoordEnd).end()) {
                // associate updated vector with the geoCoord
                vector<StreetSegment> newOne = (*testCoordEnd);
                newOne.push_back(whoDat);
                m_geoCoords.associate(whoDat.segment.end, newOne);
            }
        }
        
        // loop through attractions and check to see if they are in the map
        for(int j = 0; j < whoDat.attractions.size(); j++) {
            // checks to see if ending geoCoords are already in the tree
            const vector<StreetSegment>* aCoord = m_geoCoords.find(whoDat.attractions[j].geocoordinates);
            if(aCoord == nullptr) {
                vector<StreetSegment> attractionCoord;
                attractionCoord.push_back(whoDat);
                m_geoCoords.associate(whoDat.attractions[j].geocoordinates, attractionCoord);
            }
            // association already exists for that geoCoord
            else {
                // make sure this segment is not already in the vector
                vector<StreetSegment>::const_iterator it = (*aCoord).begin();
                for(; it < (*aCoord).end(); it++) {
                    // street segment already exists
                    if((*it).streetName == whoDat.streetName &&
                       (*it).segment.start.latitudeText == whoDat.segment.start.latitudeText &&
                       (*it).segment.start.longitudeText == whoDat.segment.start.longitudeText &&
                       (*it).segment.end.latitudeText == whoDat.segment.end.latitudeText &&
                       (*it).segment.start.longitudeText == whoDat.segment.start.longitudeText
                       )
                        break;
                }
                // if nullptr, then the StreetSegment was not found
                if(it == (*aCoord).end()) {
                    // associate updated vector with the geoCoord
                    vector<StreetSegment> newOne = (*aCoord);
                    newOne.push_back(whoDat);
                    m_geoCoords.associate(whoDat.attractions[j].geocoordinates, newOne);
                }
            }
        }
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    // use find function from MyMap class
    const vector<StreetSegment>* found = m_geoCoords.find(gc);
    // if nullptr then was not found
    if(found == nullptr) {
        vector<StreetSegment> empty;
        return empty;
    }
    return *found;
}


// ------------------- Operation Overloads -----------------------

// less than operator
bool operator<(const GeoCoord& lhs, const GeoCoord& rhs) {
    if(lhs.latitude == rhs.latitude)
        return lhs.longitude < rhs.longitude;
    return lhs.latitude < rhs.latitude;
}

// greater than operator
bool operator>(const GeoCoord& lhs, GeoCoord& rhs) {
    if(lhs.latitude == rhs.latitude)
        return lhs.longitude > rhs.longitude;
    return lhs.latitude > rhs.latitude;
}

// equal to operator
bool operator==(const GeoCoord& lhs, const GeoCoord& rhs) {
    return lhs.latitude == rhs.latitude && lhs.longitude == rhs.longitude;
}


//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
