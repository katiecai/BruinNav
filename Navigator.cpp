//#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <string>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;

struct Node {
    Node(const GeoCoord cur, const GeoCoord dest, Node* p, double distance) : m_cur(cur.latitudeText, cur.longitudeText) {
        h = distanceEarthMiles(cur, dest);
        g = distance;
        f = g + h;
        parent = p;
        //if queue is empty, pass in nullptr
        if (parent == nullptr)
            path.push_back(cur);
        else {
            path = parent->path;
            path.push_back(cur);
        }
    }
    Node() {
    }
    GeoCoord m_cur;
    double f;
    double g;
    double h;
    Node* parent;
    Node& operator=(const Node& rhs);
    vector<GeoCoord> path;
};

Node& Node::operator=(const Node& rhs) {
    if (this != &rhs) {
        m_cur = rhs.m_cur;
        f = rhs.f;
        g = rhs.g;
        h = rhs.h;
        path = rhs.path;
        parent = rhs.parent;
    }
    return *this;
}

bool operator==(const Node& lhs, const Node& rhs) {
    if (lhs.f == rhs.f)
        return true;
    return false;
}

bool operator<(const Node& lhs, const Node& rhs) {
    if (lhs.f > rhs.f)
        return true;
    return false;
}

bool operator>(const Node& lhs, const Node& rhs) {
    if (lhs.f < rhs.f)
        return true;
    return false;
}

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
    string getDirection(double angle) const;
    string getTurnDirection(double angle) const;
private:
    MapLoader ml;
    AttractionMapper am;
    SegmentMapper sm;
};

NavigatorImpl::NavigatorImpl()
{
    
}

NavigatorImpl::~NavigatorImpl()
{
}

string NavigatorImpl::getDirection(double angle) const {
    if (angle >= 0 && angle <= 22.5)
        return "east";
    else if (angle > 22.5 && angle <= 67.5)
        return "northeast";
    else if (angle > 67.5 && angle <= 112.5)
        return "north";
    else if (angle > 112.5 && angle <= 157.5)
        return "northwest";
    else if (angle > 157.5 && angle <= 202.5)
        return "west";
    else if (angle > 202.5 && angle <= 247.5)
        return "southwest";
    else if (angle > 247.5 && angle <= 292.5)
        return "south";
    else if (angle > 292.5 && angle <= 337.5)
        return "southeast";
    return "east";
}

string NavigatorImpl::getTurnDirection(double angle) const {
    if (angle < 180)
        return "left";
    else
        return "right";
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    MapLoader ml;
    if (ml.load(mapFile)) {
        am.init(ml);
        sm.init(ml);
        return true;
    }
    return false;// This compiles, but may not be correct
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    //A*
    MyMap<GeoCoord, Node> closed;
    priority_queue<Node> open;
    
    GeoCoord begin;
    GeoCoord dest;
    
    if (!am.getGeoCoord(start, begin))
        return NAV_BAD_SOURCE;
    if (!am.getGeoCoord(end, dest))
        return NAV_BAD_DESTINATION;
    
    //push starting coord onto open and closed lists
    Node first(begin, dest, nullptr, 0);
    open.push(first);
    closed.associate(first.m_cur, first);
    
    if (first.m_cur == dest)
        return NAV_SUCCESS;
    
    while (!open.empty()) {
        Node current = open.top();
        open.pop();
    
        
        //found a path of geocoords -> create vector of streetsegments -> create vector of navsegments
        if (current.m_cur == dest) {
            closed.associate(current.m_cur, current);
            cerr << closed.find(dest)->path.size() << endl;
            

            vector<GeoCoord> gcPath = closed.find(dest)->path;
            vector<StreetSegment> correctStreetSegs;
            
//            for (int i = 0; i < gcPath.size(); i++) {
//                cerr << gcPath[i].latitudeText << ", " << gcPath[i].longitudeText << endl;
//            }
            
            for (int j = 0; j < gcPath.size()-1; j++) {
                vector<StreetSegment> streetSegs = sm.getSegments(closed.find(dest)->path.at(j));
                for (int k = 0; k < streetSegs.size(); k++) {
                    //if the start and end geocoords of the street segment are equal to two consecutive geocoords in the path
                    //we found the correct street segment
                    if ((streetSegs[k].segment.start == gcPath[j+1] && streetSegs[k].segment.end == gcPath[j]) ||
                        (streetSegs[k].segment.start == gcPath[j] && streetSegs[k].segment.end == gcPath[j+1])) {
                        cerr << "lol";
                        StreetSegment temp;
                        temp.segment.start = gcPath[j];
                        temp.segment.end = gcPath[j+1];
                        temp.streetName = streetSegs[k].streetName;
                        correctStreetSegs.push_back(temp);
                        break;
                    }
                    //loop through attractions to see if the start/end attractions are in the middle of a street
                    for (int l = 0; l < streetSegs[k].attractions.size(); l++) {
                        if ((gcPath[j] == streetSegs[k].attractions[l].geocoordinates && gcPath[j+1] == streetSegs[k].segment.end) ||
                            (gcPath[j] == streetSegs[k].attractions[l].geocoordinates && gcPath[j+1] == streetSegs[k].segment.start) ||
                            (gcPath[j+1] == streetSegs[k].attractions[l].geocoordinates && gcPath[j] == streetSegs[k].segment.end) ||
                            (gcPath[j+1] == streetSegs[k].attractions[l].geocoordinates && gcPath[j] == streetSegs[k].segment.start)) {
                            cerr << "lol";
                            StreetSegment temp;
                            temp.segment.start = gcPath[j];
                            temp.segment.end = gcPath[j+1];
                            temp.streetName = streetSegs[k].streetName;
                            correctStreetSegs.push_back(temp);
                            break;
                        }
                    }
                }
            }
            
            //push first direction onto vector
            //if (correctStreetSegs.size() > 0) {
            string firstDirection = getDirection(angleOfLine(correctStreetSegs[0].segment));
            directions.push_back(NavSegment(firstDirection, correctStreetSegs[0].streetName, distanceEarthMiles(correctStreetSegs[0].segment.start, correctStreetSegs[0].segment.end), correctStreetSegs[0].segment));
            //}
            
            //loop thru rest of street segments
            for (int l = 1; l < correctStreetSegs.size(); l++) {
                string dir = getDirection(angleOfLine(correctStreetSegs[l].segment));
                double dist = distanceEarthMiles(correctStreetSegs[l].segment.start, correctStreetSegs[l].segment.end);
                if (correctStreetSegs[l].streetName == correctStreetSegs[l-1].streetName)
                    directions.push_back(NavSegment(dir, correctStreetSegs[l].streetName, dist, correctStreetSegs[l].segment));
                else {
                    directions.push_back(NavSegment(getTurnDirection(angleBetween2Lines(correctStreetSegs[l-1].segment, correctStreetSegs[l].segment)), correctStreetSegs[l].streetName));
                    directions.push_back(NavSegment(dir, correctStreetSegs[l].streetName, dist, correctStreetSegs[l].segment));
                }
            }
            
            for (int x = 0; x < directions.size(); x++) {
                cerr << directions[x].m_streetName << endl;
                cerr << directions[x].m_direction << endl;
                cerr << directions[x].m_distance << endl;
            }
//            
            cerr << "success" << endl;
            return NAV_SUCCESS;
        }
        
        vector<StreetSegment> streets = sm.getSegments(current.m_cur);
        for (int i = 0; i < streets.size(); i++) {
            GeoCoord gc;
            GeoCoord gc2;
            
            bool isThereGc2 = false;
            
            if (streets[i].segment.end == current.m_cur)
                gc = streets[i].segment.start;
            else if (streets[i].segment.start == current.m_cur)
                gc = streets[i].segment.end;
            else {
                gc = streets[i].segment.start;
                gc2 = streets[i].segment.end;
                isThereGc2 = true;
            }
            
            //check attractions in case attraction destination is in the middle of the street
            vector<Attraction> a = streets[i].attractions;
            for (int j = 0; j < a.size(); j++) {
                if (a[j].geocoordinates == dest)
                    gc = a[j].geocoordinates;
            }
            
            Node neighbor(gc, dest, &current, current.g + distanceEarthMiles(gc, current.m_cur));
            
            if (closed.find(neighbor.m_cur) == nullptr || (closed.find(neighbor.m_cur) != nullptr &&
                neighbor.f < closed.find(neighbor.m_cur)->f))
                open.push(neighbor);
            if (isThereGc2)
                open.push(Node(gc2, dest, &current, current.g + distanceEarthMiles(gc2, current.m_cur)));
            
            closed.associate(current.m_cur, current);
        }
    }
    return NAV_NO_ROUTE;
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
