#include "provided.h"
#include <string>
#include <iostream>
#include "MyMap.h"
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string, GeoCoord> map;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    for (int i = 0; i < ml.getNumSegments(); i++) {
        StreetSegment s;
        ml.getSegment(i, s);
        for (int j = 0; j < s.attractions.size(); j++) {
            for (int k = 0; k < s.attractions[j].name.size(); k++)
                s.attractions[j].name[k] = tolower(s.attractions[j].name[k]);
            map.associate(s.attractions[j].name, s.attractions[j].geocoordinates);
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    for (int i = 0; i < attraction.size(); i++) {
        attraction[i] = tolower(attraction[i]);
    }
    
    const GeoCoord* g = map.find(attraction);
    if (g != nullptr) {
        gc = *g;
        return true;
    }
    
	return false;  // This compiles, but may not be correct
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
