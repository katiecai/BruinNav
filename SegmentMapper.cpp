//#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <vector>
#include <iostream>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
    
private:
    MyMap<GeoCoord, vector<StreetSegment>> segmentMap;
    
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    for (int i = 0; i < ml.getNumSegments(); i++) {
        StreetSegment s;
        //get the segment
        ml.getSegment(i, s);
        //get the start, end geocoords
        GeoCoord start = s.segment.start;
        GeoCoord end = s.segment.end;
        vector<GeoCoord> attractioncoords;
        
        //obtain the attractions of the segment
        for (int j = 0; j < s.attractions.size(); j++) {
            attractioncoords.push_back(s.attractions[j].geocoordinates);
        }
    
        //find the segment vector associated with the start geocoord
        vector<StreetSegment>* segmentsPtr = segmentMap.find(start);
        
        //Starting Coordinate
        //map the geocoord if it's not already on there
        if (segmentsPtr == nullptr) {
            vector<StreetSegment> streetSegments;
            streetSegments.push_back(s);
            segmentMap.associate(start, streetSegments);
        }
        //otherwise, push the street segment onto the street seg vector
        else {
            bool canPush = true;
            //loop thru vector of street segments to check if the street segment is already in the vector before pushing
            for (int i = 0; i < segmentsPtr->size(); i++) {
                if (s == segmentsPtr->at(i))
                    canPush = false;
            }
            
            if (canPush == true)
                segmentsPtr->push_back(s);
        }
        
        //Ending Coordinate
        segmentsPtr = segmentMap.find(end);
        if (segmentsPtr == nullptr) {
            vector<StreetSegment> streetSegments;
            streetSegments.push_back(s);
            segmentMap.associate(end, streetSegments);
        }
        else {
            bool canPush = true;
            for (int i = 0; i < segmentsPtr->size(); i++) {
                if (s == segmentsPtr->at(i))
                    canPush = false;
            }
            
            if (canPush == true)
                segmentsPtr->push_back(s);
        }
        
        //Attractions
        for (int k = 0; k < attractioncoords.size(); k++) {
            segmentsPtr = segmentMap.find(attractioncoords[k]);
            if (segmentsPtr == nullptr) {
                vector<StreetSegment> streetSegments;
                streetSegments.push_back(s);
                segmentMap.associate(attractioncoords[k], streetSegments);
            }
            else {
                bool canPush = true;
                for (int i = 0; i < segmentsPtr->size(); i++) {
                    if (s == segmentsPtr->at(i))
                        canPush = false;
                }
                
                if (canPush == true)
                    segmentsPtr->push_back(s);
            }
        }
    }
    //cerr << segmentMap.size() << endl;
     
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    const vector<StreetSegment> empty;
    const vector<StreetSegment>* streetSegment = segmentMap.find(gc);
    if (streetSegment != nullptr)
        return *streetSegment;
    return empty;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want                  vcb    to change any of this code.

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
