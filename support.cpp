//
//  support.cpp
//  project 4
//
//  Created by Katie Cai on 3/13/17.
//  Copyright © 2017 Katie Cai. All rights reserved.
//

#include "support.h"

bool operator==(const StreetSegment& lhs, const StreetSegment& rhs) {
    if (lhs.streetName != rhs.streetName)
        return false;
    if (lhs.segment.start.latitudeText != rhs.segment.start.latitudeText)
        return false;
    if (lhs.segment.end.latitudeText != rhs.segment.end.latitudeText)
        return false;
    if (lhs.segment.start.longitudeText != rhs.segment.start.longitudeText)
        return false;
    if (lhs.segment.end.latitudeText != rhs.segment.end.latitudeText)
        return false;
    return true;
}

//>, <, == for geocoords

bool operator==(const GeoCoord& lhs, const GeoCoord& rhs) {
    if (lhs.latitude == rhs.latitude && lhs.longitude == rhs.longitude)
        return true;
    return false;
}

bool operator>(const GeoCoord& lhs, const GeoCoord& rhs) {
    if (lhs.latitude > rhs.latitude)
        return true;
    else if (rhs.latitude > lhs.latitude)
        return false;
    else if (lhs.longitude > rhs.longitude)
        return true;
    else if (rhs.longitude > lhs.longitude)
        return false;
    return false;
}

bool operator<(const GeoCoord& lhs, const GeoCoord& rhs) {
    if (lhs.latitude < rhs.latitude)
        return true;
    else if (rhs.latitude < lhs.latitude)
        return false;
    else if (lhs.longitude < rhs.longitude)
        return true;
    else if (rhs.longitude < lhs.longitude)
        return false;
    return false;
}
