//
//  OptionGreeks-Test.cpp
//  QuantLib
//
//  Created by Chris Len on 11/2/14.
//  Copyright (c) 2014 Chris Len. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "catch.hpp"
#include "OptionGreeks.h"
#include "Utilities.h"

TEST_CASE("Options Greeks")
{
    
    /*--------------------------------Delta---------------------------------------*/
    SECTION("Calc delta Black-Scholes-Merton on European option")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double result = 0.5216; // Value is derived from chapter 18 of Options, Future,
                                // and other Derivatives 8th edition
        auto start = std::chrono::system_clock::now();
        double delta = OptionGreeks::calcDeltaBlackScholes(s0, k, r, vol, T);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(delta, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        std::cout << "Delta Black-Scholes-Merton took " << elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds" << std::endl;
    }
    
    SECTION("Calc delta numerically on European option")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double result = 0.5216; // Value is derived from chapter 18 of Options, Future,
                                // and other Derivatives 8th edition
        
        auto start = std::chrono::system_clock::now();
        double delta = OptionGreeks::calcDeltaNumerically(s0, k, r, vol, T);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(delta, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Delta numerically took " << elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds" << std::endl;
    }
    
    /*--------------------------------Theta---------------------------------------*/
    
    SECTION("Calc theta Black-Scholes-Merton on European call option")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double result = -4.30539; // Value is derived from chapter 18 of Options, Future,
                                  // and other Derivatives 8th edition
        
        auto start = std::chrono::system_clock::now();
        double theta = OptionGreeks::calcThetaCallBlackScholes(s0, k, r, vol, T);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(theta, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Theta call Black-Scholes-Merton took " << elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds" << std::endl;
    }
    
    SECTION("Calc theta Black-Scholes-Merton on European put option")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double result = -1.853; // Value is derived from chapter 18 of Options, Future,
                                // and other Derivatives 8th edition
        
        auto start = std::chrono::system_clock::now();
        double theta = OptionGreeks::calcThetaPutBlackScholes(s0, k, r, vol, T);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(theta, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Theta put Black-Scholes-Merton took " << elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds" << std::endl;
    }
    
    SECTION("Calc theta numerically on European option")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double result = 4.30539; // Note: Theta is usually negative, but we are verifying numerical
                                 // solution is the right number, if we were to use this we would
                                 // both adjust its sign and duration convention (either theta per
                                 // calendar day or
        
        auto start = std::chrono::system_clock::now();
        double theta = OptionGreeks::calcThetaNumerically(s0, k, r, vol, T);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(theta, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Theta numerically took " << elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds" << std::endl;
    }
    
    SECTION("Relationship between calls and puts when calculating theta")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double thetaCall = OptionGreeks::calcThetaCallBlackScholes(s0, k, r, vol, T);
        double thetaPut = OptionGreeks::calcThetaPutBlackScholes(s0, k, r, vol, T);
        
        // Since N(-d2) == 1 - N(d2), the theta of a put exceeds the call
        // by exactly rKe^-rt
        
        double diff = r * k * exp(-1 * r * T);
        
        REQUIRE(Utilities::closeEnough(thetaPut - thetaCall, diff));
    }
    
    /*--------------------------------Gamma---------------------------------------*/

    
    SECTION("Calc gamma Black-Scholes-Merton on European option")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double result = 0.065545; // Value is derived from chapter 18 of Options, Future,
                                  // and other Derivatives 8th edition
        
        auto start = std::chrono::system_clock::now();
        double gamma = OptionGreeks::calcGammaBlackScholes(s0, k, r, vol, T);
        auto end = std::chrono::system_clock::now();
        REQUIRE(Utilities::closeEnough(gamma, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Gamma Black-Scholes-Merton took " << elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds" << std::endl;
    }
    
    SECTION("Calc gamma numerically on European option")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double result = 0.065545; // Value is derived from chapter 18 of Options, Future,
                                  // and other Derivatives 8th edition
        
        auto start = std::chrono::system_clock::now();
        double gamma = OptionGreeks::calcGammaNumerically(s0, k, r, vol, T);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(gamma, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Gamma numerically took " << elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds" << std::endl;
    }
    
    /*--------------------------------Vega---------------------------------------*/

    
    SECTION("Calc vega Black-Scholes-Merton on European option")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double result = 12.1;  // Value is derived from chapter 18 of Options, Future,
                               // and other Derivatives 8th edition
        
        auto start = std::chrono::system_clock::now();
        double vega = OptionGreeks::calcVegaBlackScholes(s0, k, r, vol, T);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(vega, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Vega Black-Scholes-Merton took " << elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds" << std::endl;
    }
    
    SECTION("Calc vega numerically on European option")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double result = 12.1; // Value is derived from chapter 18 of Options, Future,
                              // and other Derivatives 8th edition
        
        auto start = std::chrono::system_clock::now();
        double vega = OptionGreeks::calcVegaNumerically(s0, k, r, vol, T);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(vega, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Vega numerically took " << elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds" << std::endl;
    }
    
    SECTION("Calc rho Black-Scholes-Merton on European call option")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double result = 8.9066; // Value is derived from chapter 18 of Options, Future,
                              // and other Derivatives 8th edition
        
        auto start = std::chrono::system_clock::now();
        double rho = OptionGreeks::calcRhoCallBlackScholes(s0, k, r, vol, T);
        auto end = std::chrono::system_clock::now();

        REQUIRE(Utilities::closeEnough(rho, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Rho Black-Scholes-Merton took " << elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds" << std::endl;
    }
    
    SECTION("Calc rho numerically on European option")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double result = 8.9066; // Value is derived from chapter 18 of Options, Future,
        // and other Derivatives 8th edition
        
        auto start = std::chrono::system_clock::now();
        double rho = OptionGreeks::calcRhoNumerically(s0, k, r, vol, T);
        auto end = std::chrono::system_clock::now();

        REQUIRE(Utilities::closeEnough(rho, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Rho numerically took " << elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds" << std::endl;
    }
    
}