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
    
    // Template functions, used to compare speeds
    template <typename Func>
    double valueImpliedVolatilityIterativeTemplate(Func,double,double,double,double,double);
    template <typename Func>
    double valueImpliedVolatilityBisectionTemplate(Func,double,double,double,double,double);
    template <typename Func>
    double valueImpliedVolatilityNewtonRaphsonTemplate(Func,double,double,double,double,double);
};

/*------------------------------Template Functions----------------------------*/
// Note: Template functions must be declared in .h file.
// Used to compare speed vs declaring function via std::function


// Calculate value of option iteratively
// Uses values pass into constructor to determine implied volatility
//
// Iterative version, utilizing the KISS approach
template <typename Func>
double BlackScholesMerton::valueImpliedVolatilityIterativeTemplate(Func func,
                                                           double s0, double k, double r, double T, double c)
{
    const double INCREMENT = 0.0001; // Go up by 1bps each iteration
    
    double x = 0.0; // sample vol, start at 0 and work up to 1 (100%).
    for(int i = 0; i < MAX_ITERATIONS; i++)
    {
        // Try current value of volatility
        // Note, we are not evaluating with dividends
        double res = func(s0,k,r,x,T,0,{});
        if(Utilities::isWithin(c,res,TOLERANCE))
        {
            return x;
        }
        // We were not close enough to actual result, try again
        x = x + INCREMENT;
        
    }
    std::cerr << "Implied volatility not calculated";
    return 0.0;
}


// Uses bisection method to calculate implied volatility
template <typename Func>
double BlackScholesMerton::valueImpliedVolatilityBisectionTemplate(Func func,
                                                           double s0, double k, double r, double T, double c)
{
    double high = 1.0; // Max volatility possible is 100%
    double low = 0.0; // Min volatility possible is 0%
    
    double f_high,f_low,mid;
    
    double n = 0;
    while(n < MAX_ITERATIONS)
    {
        mid = 0.5 * (high + low);
        double res = func(s0,k,r,mid,T,0,{}) - c;
        
        // If we get 'close enough' to a right answer
        if(std::abs(res) < TOLERANCE || (0.5*(high - low) < TOLERANCE))
        {
            return mid;
        }
        n = n + 1;
        f_high = func(s0,k,r,high,T,0,{}) - c;
        f_low = func(s0,k,r,low,T,0,{}) - c;
        if(f_high*res > 0) // same sign
        {
            high = mid;
        }
        else
        {
            low = mid;
        }
    }
    std::cerr << "Bisection method failed";
    return 0.0;
}


//double differentiate(std::function<double (double)> func, double x)
template <typename Func>
double differentiate(Func func, double x)
{
    double EPS = 0.001;
    double b = func(x + EPS);
    double a = func(x - EPS);
    
    return (b-a) / (2.0*EPS);
}

template <typename Func>
double BlackScholesMerton::valueImpliedVolatilityNewtonRaphsonTemplate(Func func2,double s0, double k, double r, double T, double c)
{
    auto option = [&] (double x) -> double { return func2(s0,k,r,x,T,0,{}); };
    auto vega = [&] (double x) -> double { return differentiate(option, x); };
    
    // Start with volatility of 15%
    double v = 0.15;
    double y = func2(s0,k,r,v,T,0,{});
    
    while(std::abs(y - c) > TOLERANCE)
    {
        double d_x = vega(v);
        v += (c-y)/d_x;
        y = option(v);
    }
    
    return v;
}



#endif /* defined(__QuantLib__Black_Scholes_Merton__) */
