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
    SECTION("Calc delta on European option")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double result = 0.5216; // Value is derived from chapter 18 of Options, Future,
                               // and other Derivitives 8th edition
        auto start = std::chrono::system_clock::now();
        double delta = OptionGreeks::calcDelta(s0, k, r, vol, T);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(delta, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        std::cout << "Delta took " << elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds" << std::endl;
    }
    
    SECTION("Calc delta numerically on European option")
    {
        double s0 = 49;
        double k = 50;
        double vol = 0.2;
        double r = 0.05;
        double T = 0.3846;
        
        double result = 0.5216; // Value is derived from chapter 18 of Options, Future,
        // and other Derivitives 8th edition
        
        auto start = std::chrono::system_clock::now();
        double delta = OptionGreeks::calcDeltaNumerically(s0, k, r, vol, T);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(delta, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Delta numerically took " << elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds" << std::endl;
    }
    
}