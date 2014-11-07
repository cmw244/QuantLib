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

/*--------------------------------Delta---------------------------------------*/

// Calculate delta from the Black-Scholes formula
double OptionGreeks::calcDeltaBlackScholes(double s0, double k, double r, double vol, double T)
{
    double d1 = BlackScholesMerton::calcD1(s0, k, r, vol, T);
    
    return boost::math::cdf(BlackScholesMerton::normal, d1);
}

double OptionGreeks::calcDeltaNumerically(double s0, double k, double r, double vol, double T)
{
    auto func = [&] (double x) -> double { return BlackScholesMerton::valueCallOption(x, k, r, vol, T); };
    
    return Utilities::differentiate(func, s0);
    
}




/*--------------------------------Theta---------------------------------------*/

double nPrime(double d)
{
    return (1.0 / sqrt(2 * M_PI)) * exp((-1 * d * d) / 2);
}

double OptionGreeks::calcThetaCallBlackScholes(double s0, double k, double r, double vol, double T)
{
    double d1 = BlackScholesMerton::calcD1(s0, k, r, vol, T);
    double d2 = d1 - (vol * sqrt(T));
    
    double a = (s0 * nPrime(d1) *  vol) / (2 * sqrt(T));
    double b = r * k * exp(-1 * r * T) * cdf(BlackScholesMerton::normal, d2);
    
    return -a - b;
}

double OptionGreeks::calcThetaPutBlackScholes(double s0, double k, double r, double vol, double T)
{
    double d1 = BlackScholesMerton::calcD1(s0, k, r, vol, T);
    double d2 = d1 - (vol * sqrt(T));
    
    double a = (s0 * nPrime(d1) *  vol) / (2 * sqrt(T));
    double b = r * k * exp(-1 * r * T) * cdf(BlackScholesMerton::normal, -d2);
    
    return -a + b;
}

double OptionGreeks::calcThetaNumerically(double s0, double k, double r, double vol, double T)
{
    auto func = [&] (double x) -> double { return BlackScholesMerton::valueCallOption(s0, k, r, vol, x); } ;
    
    return Utilities::differentiate(func, T);
}


/*--------------------------------Gamma---------------------------------------*/

double OptionGreeks::calcGammaBlackScholes(double s0, double k, double r, double vol, double T)
{
    double d1 = BlackScholesMerton::calcD1(s0, k, r, vol, T);
    double numerator = nPrime(d1);
    double denominator = s0 * vol * sqrt(T);
    
    return numerator / denominator;
}

double OptionGreeks::calcGammaNumerically(double s0, double k, double r, double vol, double T)
{
    auto func = [&] (double x) -> double { return calcDeltaNumerically(x, k, r, vol, T); };
    
    return Utilities::differentiate(func, s0);
}


/*--------------------------------Vega----------------------------------------*/

double OptionGreeks::calcVegaBlackScholes(double s0, double k, double r, double vol, double T)
{
    double d1 = BlackScholesMerton::calcD1(s0, k, r, vol, T);
    
    return s0 * sqrt(T) * nPrime(d1);
}

double OptionGreeks::calcVegaNumerically(double s0, double k, double r, double vol, double T)
{
    auto func = [&] (double x) -> double { return BlackScholesMerton::valueCallOption(s0, k, r, x, T); };
    
    return Utilities::differentiate(func, vol);
}


