#include "provided.h"
#include <string>
#include <fstream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    int m_size;
    vector<StreetSegment> m_segments;
    
    // string tokenize function helper
    void tokenize(string splitUp, char limit[], int limitSize, vector<string>& strings);
};

MapLoaderImpl::MapLoaderImpl()
{
    m_size = 0;
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{

    ifstream mapData; // Create file stream and open it
    mapData.open(mapFile);
    
    if (mapData.is_open())
    {
        string line;
        int lineNum = 0;
        while ( getline (mapData,line) ) // Go through every line in the file
        {
            
            if(lineNum == 0) {
                // create new street segment to put into vector
                StreetSegment next;
                m_segments.push_back(next);
                m_size++;
                
                // set the street segment name
                m_segments[m_segments.size()-1].streetName = line;
                
                lineNum++;
            }
            else if(lineNum == 1) {
                // tokenize the line that has the geo coordinates
                char limit[] = {','};
                vector<string> coords;
                tokenize(line, limit, 1, coords);
                
                // create the starting and ending geo coordinates
                GeoCoord start(coords[0], coords[1]);
                GeoCoord end(coords[2], coords[3]);
                
                // create the new geosegment with the geocoordinates
                GeoSegment next(start, end);
                m_segments[m_segments.size()-1].segment = next;
                
                lineNum++;
            }
            else {
                int numAttractions = stoi(line);
                for(int i = 0; i < numAttractions; i++) {
                    // get line that has the attraction in it
                    getline (mapData,line);
                    
                    // create an attraction to put into the attraction vector
                    Attraction next;
                    next.name = line.substr(0, line.find('|'));
                    // tokenize geocoordinates
                    char limit[] = {','};
                    vector<string> coords;
                    tokenize(line.substr(line.find('|')+1), limit, 1, coords);
                    // put geocoordinates in the attraction
                    GeoCoord location(coords[0], coords[1]);
                    next.geocoordinates = location;
                    
                    // push Attraction onto Attraction vector
                    m_segments[m_segments.size()-1].attractions.push_back(next);
                }
                lineNum = 0;
            }
        }
        mapData.close();
    }
    
    else
        return false; // Return false if file cannot be opened
    
	return true;  // Return true when done
}

size_t MapLoaderImpl::getNumSegments() const
{
    return m_size; // Return the size of the private member variable
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if(segNum >= getNumSegments())
        return false;  // Return false if the number is outside the limits of the vector
    seg = m_segments[segNum]; // set parameter to
    return true;
}


// string tokenizer function to help
void MapLoaderImpl::tokenize(string splitUp, char limit[], int limitSize, vector<string>& strings) {
    int startPos = 0;
    int currPos = 0;
    while(startPos < splitUp.size() && currPos < splitUp.size()) {
        for(int i = 0; i < limitSize; i++) {
            if(splitUp[currPos] == limit[i] || isspace(splitUp[currPos])) {
                if(startPos == currPos) {
                    startPos++;
                    break;
                }
                strings.push_back(splitUp.substr(startPos, currPos-startPos));
                startPos = currPos+1;
                break;
            }
        }
        currPos++;
    }
    if(startPos < splitUp.size()) {
        strings.push_back(splitUp.substr(startPos, splitUp.size()-startPos));
    }
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
