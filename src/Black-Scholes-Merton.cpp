//
//  Black-Scholes-Merton.cpp
//  QuantLib
//
//  Created by Chris Len on 10/25/14.
//  Copyright (c) 2014 Chris Len. All rights reserved.
//

#include <boost/math/distributions.hpp>
#include "Black-Scholes-Merton.h"
#include "Utilities.h"

using namespace boost::math;
using namespace std;

// @param s: Stock price at time of valuation
// @param k: Strike price of option being valued
// @param r: riskless rate, assumed to be contstant over duration of option (in decimal %)
// @param vol: volatility of underlying stock (in decimal %)
// @param T: duration of option (in years)
// @param dividend: Dividend payment, schedule must be set using setter method
double BlackScholesMerton::valueCallOption(double s0, double k, double r, double vol,
                                           double T, double dividend, vector<double> paymentSchedule)
{
    if(!paymentSchedule.empty()) // We need to adjust s0 for dividends
    {
        double adjust = 0.0;
        
        for(int i = 0; i < paymentSchedule.size(); i++)
        {
            // Discount future payment at riskless rate
            adjust = adjust + dividend * exp(-1 * r * paymentSchedule[i]);
        }
        s0 = s0 - adjust;
    }
    double d1 = calcD1(s0,k,r,vol,T);
    double d2 = d1 - (vol * sqrt(T)); // Optimization to avoid a couple of computations
    // Equivalent to (log(s0 / k) + (r - vol*vol/2) * T) / (vol * sqrt(T))
    
    // Valuations based off equation c = s0 * N(d1) - K * e^-rT * N(d2)
    // where N(x) is the cumulative probability distribution for a standardized normal distribution
    
    return s0 * cdf(normal, d1) - k * exp(-1 * r * T)* cdf(normal, d2);
}



// Helper method that is abstracted because it can be reused (such as delta on options)
double BlackScholesMerton::calcD1(double s0, double k, double r, double vol, double T)
{
    return (log(s0 / k) + (r + vol*vol/2) * T) / (vol * sqrt(T));
}

double BlackScholesMerton::valuePutOption(double s0, double k, double r, double vol,
                                          double T, double dividend, vector<double> paymentSchedule)
{
    if(!paymentSchedule.empty()) // We need to adjust s0 for dividends
    {
        double adjust = 0.0;
        
        for(int i = 0; i < paymentSchedule.size(); i++)
        {
            // Discount future payment at riskless rate
            adjust = adjust + dividend * exp(-1 * r * paymentSchedule[i]);
        }
        s0 = s0 - adjust;
    }
    double d1 = (log(s0 / k) + (r + vol*vol/2) * T) / (vol * sqrt(T));
    double d2 = d1 - (vol * sqrt(T)); // Optimization to avoid a couple of computations
    // Equivalent to (log(s0 / k) + (r - vol*vol/2) * T) / (vol * sqrt(T))
    
    
    // Valuations based off equation c = s0 * N(d1) - K * e^-rT * N(d2)
    // where N(x) is the cumulative probability distribution for a standardized normal distribution
    
    return k * exp(-1 * r * T)* cdf(normal, -d2) - s0 * cdf(normal, -d1);
}

// Calculate value of option iteratively
// Uses values pass into constructor to determine implied volatility
//
// Iterative version, utilizing the KISS approach

double BlackScholesMerton::valueImpliedVolatilityIterative(std::function<double (double, double, double, double, double, double, std::vector<double>)> func,
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
    cerr << "Implied volatility not calculated";
    return 0.0;
}


// Uses bisection method to calculate implied volatility
double BlackScholesMerton::valueImpliedVolatilityBisection(function<double (double, double, double, double, double, double, vector<double>)> func,
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
        if(abs(res) < TOLERANCE || (0.5*(high - low) < TOLERANCE))
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
    cerr << "Bisection method failed";
    return 0.0;
}


double BlackScholesMerton::valueImpliedVolatilityNewtonRaphson(function<double (double, double, double, double, double, double, vector<double>)> func2,
                                                               double s0, double k, double r, double T, double c)
{
    auto option = [&] (double x) -> double { return func2(s0,k,r,x,T,0,{}); };
    auto vega = [&] (double x) -> double { return differentiate(option, x); };
    
    // Start with volatility of 15%
    double v = 0.15;
    double y = func2(s0,k,r,v,T,0,{});
    
    while(abs(y - c) > TOLERANCE)
    {
        double d_x = vega(v);
        v += (c-y)/d_x;
        y = option(v);
    }
    
    return v;
}



