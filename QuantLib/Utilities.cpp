//
//  Utilities.cpp
//  QuantLib
//
//  Created by Chris Len on 10/29/14.
//  Copyright (c) 2014 Chris Len. All rights reserved.
//

#include <stdio.h>
#include "Utilities.h"

// Used to allow some precision error in results
bool Utilities::closeEnough(double a, double b)
{
    const double verySmallNumber = 0.0001;

    if ( b == 0 )
    {
        return (a - verySmallNumber) < b &&
            (a + verySmallNumber) > b;
    }

    double r = a / b;
    return ( r + verySmallNumber > 1 &&
            r - verySmallNumber < 1 );
}

bool Utilities::isWithin(double a, double b, double threshold)
{
    if(a > b)
    {
        return b + threshold >= a;
    }
    else
    {
        return a + threshold >= b;
    }
}

