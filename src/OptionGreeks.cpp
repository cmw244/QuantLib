//
//  OptionGreeks.cpp
//  QuantLib
//
//  Created by Chris Len on 11/2/14.
//  Copyright (c) 2014 Chris Len. All rights reserved.
//

#include "OptionGreeks.h"
#include "Black-Scholes-Merton.h"
#include <boost/math/distributions.hpp>



double OptionGreeks::calcDelta(double s0, double k, double r, double vol, double T)
{
    double d1 = BlackScholesMerton::calcD1(s0, k, r, vol, T);
    
    return boost::math::cdf(BlackScholesMerton::normal, d1);
}

double OptionGreeks::calcDeltaNumerically(double s0, double k, double r, double vol, double T)
{
    auto func = [&] (double x) -> double { return BlackScholesMerton::valueCallOption(x, k, r, vol, T); };
    
    return Utilities::differentiate(func, s0);
    
}