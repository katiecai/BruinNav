//
//  support.h
//  project 4
//
//  Created by Katie Cai on 3/13/17.
//  Copyright © 2017 Katie Cai. All rights reserved.
//

#ifndef support_h
#define support_h

#include <stdio.h>
#include "provided.h"

bool operator==(const StreetSegment& lhs, const StreetSegment& rhs);

//>, <, == for geocoords

bool operator==(const GeoCoord& lhs, const GeoCoord& rhs);

bool operator>(const GeoCoord& lhs, const GeoCoord& rhs);

bool operator<(const GeoCoord& lhs, const GeoCoord& rhs);

#endif /* support_h */
