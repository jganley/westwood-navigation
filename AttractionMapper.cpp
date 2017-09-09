#include "provided.h"
#include "MyMap.h"
#include <string>
#include <algorithm>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string, GeoCoord> m_attractions;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    // loop through all the segments in the in the map loader object
    for(int i = 0; i < ml.getNumSegments(); i++) {
        StreetSegment whoDat;
        ml.getSegment(i, whoDat);
        
        // loop through all the attractions in the segment
        for(int j = 0; j < whoDat.attractions.size(); j++) {
            
            // make the attraction name lowercase so that its case insensitive
            string place = whoDat.attractions[j].name;
            std::transform(place.begin(), place.end(), place.begin(), ::tolower);
            
            // create association between the segment and the geocoordinates
            m_attractions.associate(place, whoDat.attractions[j].geocoordinates);
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    // make the attraction name lowercase so you can find it
    string place = attraction;
    std::transform(place.begin(), place.end(), place.begin(), ::tolower);
     
    // see if an association exists for the specific attraction
    const GeoCoord* found = m_attractions.find(place);
    
    // not found, return false
    if(found == nullptr)
        return false;
    
    // if found, set paramter reference to the result and return true
	gc = *found;
    return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
