//
//  Utilities.h
//  QuantLib
//
//  Created by Chris Len on 10/29/14.
//  Copyright (c) 2014 Chris Len. All rights reserved.
//

#ifndef QuantLib_Utilities_h
#define QuantLib_Utilities_h

#include <functional>

namespace Utilities
{
    const double NANO_TO_MILLI = 100000.0;
    
    bool closeEnough(double a, double b);
    bool isWithin(double a, double b, double threshold);
    double differentiate(std::function<double (double)> func, double x);
};


#endif