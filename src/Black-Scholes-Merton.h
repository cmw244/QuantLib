//
//  Black-Scholes-Merton.h
//  QuantLib
//
//  Created by Chris Len on 10/25/14.
//  Copyright (c) 2014 Chris Len. All rights reserved.
//
//  Following file declares methods used to price European options using
//  the Black-Scholes-Merton method, which solves the Black-Scholes-Merton
//  partial differential equation.
//
//  Summary:
//  Black-Scholes-Merton model assumes portfolio consisting of:
//  1) Risky asset (usually stock)
//  2) Riskless asset (money market, bond, cash)
//
//  The following important assumetions are made in regards to market conditions:
//  - No arbitrage opportunity (no way to make a riskless profit)
//  - It is possible to borrow and lend any amount at the riskless rate
//  - It is possible to buy or sell any amount, including fractional, of the stock
//  - Transaction costs are ignored (frictionless market)
//
//  The following important assumtions are made to price the derivitive:
//  - Probability distribution of stock prices in one year are lognormal
//  - Continously compounded rate of return on the stock during the year is
//     nomally distributed
//  - The rate of return on the riskless asset is constant throughout the duration
//

#ifndef __QuantLib__Black_Scholes_Merton__
#define __QuantLib__Black_Scholes_Merton__

#include <vector>
#include <cmath>
#include "Utilities.h"
#include <boost/math/distributions.hpp>


// Class implemented as a singleton class for design experience
namespace BlackScholesMerton
{
    constexpr static double MAX_ITERATIONS = 1000000; // Used to calculate implied volatility
    constexpr static double TOLERANCE = 0.0001;  // As long as we are this close we will accept it
    
    const boost::math::normal_distribution<> normal(0.0,1.0); // Standard normal u = 0.0 and variance = 1.0
    
    double calcD1(double s0, double k, double r, double vol, double T);
    // Methods normally would be declared , but since methods are static
    // there is not this *, so const would not make sense.
    double valueCallOption(double s0, double k, double r, double vol, double T,
                           double dividend = 0.0,  std::vector<double> v = {});
    double valuePutOption(double s0, double k, double r, double vol, double T,
                          double dividend = 0.0, std::vector<double> v = {});
    
    // Imlpied volatolity method, calculated in different ways
    double valueImpliedVolatilityIterative(std::function<double (double, double, double, double, double, double, std::vector<double>)> func,
                                           double s0, double k, double r, double T, double c);
    double valueImpliedVolatilityBisection(std::function<double (double, double, double, double, double, double, std::vector<double>)>,
                                           double s0, double k, double r, double T, double c);
    double valueImpliedVolatilityNewtonRaphson(std::function<double (double, double, double, double, double, double, std::vector<double>)>,
                                               double s0, double k, double r, double T, double c);
};

#endif /* defined(__QuantLib__Black_Scholes_Merton__) */