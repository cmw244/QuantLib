//
//  OptionGreeks.h
//  QuantLib
//
//  Created by Chris Len on 11/2/14.
//  Copyright (c) 2014 Chris Len. All rights reserved.
//

#ifndef __QuantLib__OptionGreeks__
#define __QuantLib__OptionGreeks__

#include <stdio.h>

namespace OptionGreeks
{
    double calcDeltaBlackScholes(double s0, double k, double r, double vol, double T);
    double calcDeltaNumerically(double s0, double k, double r, double vol, double T);
    double calcThetaCallBlackScholes(double s0, double k, double r, double vol, double T);
    double calcThetaPutBlackScholes(double s0, double k, double r, double vol, double T);
    double calcThetaNumerically(double s0, double k, double r, double vol, double T);
}

#endif /* defined(__QuantLib__OptionGreeks__) */