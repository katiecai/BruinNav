#include "provided.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
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
    vector<StreetSegment> m_segments;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
    ifstream read(mapFile);
    if (!read) {
        return false;
    }
    
    bool finished = false;
    while (!finished)
    {
        StreetSegment s;
        
        //get street name - if it fails the text file has ended
        if (!getline(read, s.streetName)) {
            finished = true;
            break;
        }
        
        string line;
        getline(read, line);
        GeoSegment gs;
        
        string lat1;
        string long1;
        string lat2;
        string long2;
        
        //determine geocoordinates line by line
        int i = 0;
        while (i < line.size())
        {
            while (line[i] == ' ')
                i++;
            while (isdigit(line[i]) || line[i] == '.' || line[i] == '-') {
                lat1 += line[i];
                i++;
            }
            while (line[i] == ' ' || line[i] == ',')
                i++;
            while (isdigit(line[i]) || line[i] == '.' || line[i] == '-') {
                long1 += line[i];
                i++;
            }
            while (line[i] == ' ' || line[i] == ',')
                i++;
            while (isdigit(line[i]) || line[i]    == '.' || line[i] == '-') {
                lat2 += line[i];
                i++;
            }
            while (line[i] == ' ' || line[i] == ',')
                i++;
            while (isdigit(line[i]) || line[i] == '.' || line[i] == '-'){
                long2 += line[i];
                i++;
            }
        }
        
        gs.start.latitudeText = lat1;
        gs.start.longitudeText = long1;
        gs.start.latitude = stod(lat1);
        gs.start.longitude = stod(long1);
        
        gs.end.latitudeText = lat2;
        gs.end.longitudeText = long2;
        gs.end.latitude = stod(lat2);
        gs.end.longitude = stod(long2);
        
        s.segment = gs;
        
        string attractions;
        getline(read, attractions);
        string a;
        int j = 0;
        while (j < attractions.size()) {
            if (attractions[j] != ' ') {
                a += attractions[j];
            }
            j++;
        }
        
        int numOfAttractions = stoi(a);
        
        for(int k = 0; k < numOfAttractions; k++)
        {
            string attraction;
            Attraction attract;
            getline(read, attraction);
            int m = 0;
            for (m = 0; attraction[m] != '|'; m++)
                attract.name += attraction[m];
            
            GeoCoord attractionCoord;
            
            attractionCoord.latitudeText = "";
            attractionCoord.longitudeText = "";
            
            while (attraction[m] != ',') {
                if(attraction[m] != ' ' && attraction[m] != '|') {
                    attractionCoord.latitudeText += attraction[m];
                }
                m++;
            }
            
            attractionCoord.latitude = stod(attractionCoord.latitudeText);
            
            while (m < attraction.size()) {
                if (attraction[m] != ' ' && attraction[m] != ',') {
                    attractionCoord.longitudeText += attraction[m];
                }
                m++;
            }
            attractionCoord.longitude = stod(attractionCoord.longitudeText);
            attract.geocoordinates = attractionCoord;
            s.attractions.push_back(attract);
        }
        
        m_segments.push_back(s);
    }
    return true;
}


size_t MapLoaderImpl::getNumSegments() const
{
    return m_segments.size(); // This compiles, but may not be correct
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if (segNum < 0 || segNum >= getNumSegments()) {
        return false;
    }
    seg = m_segments[segNum];
    return true;// This compiles, but may not be correct
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

