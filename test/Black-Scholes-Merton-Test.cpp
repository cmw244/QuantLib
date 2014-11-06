#include <iostream>
#include "catch.hpp"
#include "Black-Scholes-Merton.h"
#include "Utilities.h"

TEST_CASE("Black-Scholes-Merton model")
{
    // Note: All result values are obtained from examples given in chapter
    // 14 of Options, Future, and Derivatives 8th edition
    
    
    //-------------------------------NO DIVIDENDS-------------------------------
 
    SECTION("Value European call option without dividends")
    {
        double stockPrice = 42.0;
        double strikePrice = 40.0;
        double risklessRate = 0.1;
        double vol = 0.2;
        double duration = 0.5;
        
        double result = 4.75942; // Value is obtained from example given in chapter
                                 // 14 of Options, Future, and Derivatives 8th edition
        
        double callOption = BlackScholesMerton::valueCallOption(stockPrice, strikePrice, risklessRate, vol, duration);
        
        REQUIRE(Utilities::closeEnough(callOption, result));
    }
    
    SECTION("Value European call option without dividends")
    {
        double stockPrice = 49.0;
        double strikePrice = 50.0;
        double risklessRate = 0.05;
        double vol = 0.2;
        double duration = 0.3846;
        
        double result = 2.4005; // Value is obtained from example given in chapter 18
                                // of Options, Future, and Derivatives 8th edition
        
        double callOption = BlackScholesMerton::valueCallOption(stockPrice, strikePrice, risklessRate, vol, duration);

        REQUIRE(Utilities::closeEnough(callOption, result));
    }
    
    SECTION("Value European put option without dividends")
    {
        double stockPrice = 42.0;
        double strikePrice = 40.0;
        double risklessRate = 0.1;
        double vol = 0.2;
        double duration = 0.5;
        
        double result = 0.808599;
        
        double putOption = BlackScholesMerton::valuePutOption(stockPrice, strikePrice, risklessRate, vol, duration);
        REQUIRE(Utilities::closeEnough(putOption, result));
    }
    
    //--------------------------------DIVIDENDS---------------------------------
    
    SECTION("Value European call option with dividends")
    {
        double stockPrice = 40.0;
        double strikePrice = 40.0;
        double risklessRate = 0.09;
        double vol = 0.3;
        double duration = 0.5;
        double dividend = 0.5;
        // Stack allocation usually faster than heap allocation
        std::vector<double> payment{1.0/6.0,5.0/12.0};
        
        double result = 3.67123; // Value is obtained from example given in chapter
                                 // 14 of Options, Future, and Derivatives 8th edition
        
        double callOption = BlackScholesMerton::valueCallOption(stockPrice, strikePrice,
                                                                risklessRate, vol, duration,
                                                                dividend, payment);
        REQUIRE(Utilities::closeEnough(callOption, result));
    }
    
    SECTION("Imlpied volatility on European call option: Iterative")
    {
        auto func = BlackScholesMerton::valueCallOption;
        
        double stockPrice = 21;
        double strikePrice = 20;
        double risklessRate = 0.1;
        double duration = 0.25;
        double optionValue = 1.875;
        
        double result = 0.2345;   // Value is obtained from example given in chapter
                                  // 14 of Options, Future, and Derivatives 8th edition
        
        
        // Keep track of performance to analyze vs. other methods of finding implied vol
        auto start = std::chrono::system_clock::now();
        double impliedVol = BlackScholesMerton::valueImpliedVolatilityIterative(func, stockPrice, strikePrice, risklessRate, duration, optionValue);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(impliedVol, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // convert manually because if we casted in milliseconds we would get 0 by rounding
        std::cout << "Iterative implied vol took " <<  elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds.\n";
    }
    
    
    SECTION("Implied volatility on European call option: Bisection method")
    {
        auto func = BlackScholesMerton::valueCallOption;
        
        double stockPrice = 21;
        double strikePrice = 20;
        double risklessRate = 0.1;
        double duration = 0.25;
        double optionValue = 1.875;
        
        double result = 0.23449;   // Value is obtained from example given in chapter
                                   // 14 of Options, Future, and Derivatives 8th edition
        
        
        // Keep track of performance to analyze vs. other methods of finding implied vol
        auto start = std::chrono::system_clock::now();
        double impliedVol = BlackScholesMerton::valueImpliedVolatilityBisection(func, stockPrice, strikePrice, risklessRate, duration, optionValue);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(impliedVol, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // convert manually because if we casted in milliseconds we would get 0 by rounding
        std::cout << "Bisection method implied vol took " <<  elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds.\n";
    }
    
    SECTION("Implied volatility on European call option: Bisection method")
    {
        auto func = BlackScholesMerton::valueCallOption;
        
        double stockPrice = 100;
        double strikePrice = 100;
        double risklessRate = 0.05;
        double duration = 1.0;
        double optionValue = 10.5;
        
        double result = 0.201355;   // Value is obtained from QuantStart
                                    // http://www.quantstart.com/articles/Implied-Volatility-in-C-using-Template-Functions-and-Interval-Bisection
        
        
        // Keep track of performance to analyze vs. other methods of finding implied vol
        auto start = std::chrono::system_clock::now();
        double impliedVol = BlackScholesMerton::valueImpliedVolatilityBisection(func, stockPrice, strikePrice, risklessRate, duration, optionValue);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(impliedVol, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // convert manually because if we casted in milliseconds we would get 0 by rounding
        std::cout << "Bisection method implied vol took " <<  elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds.\n";
    }
    
    SECTION("Implied volatility on European call option: Newton-Raphson method")
    {
        auto func = BlackScholesMerton::valueCallOption;
        
        double stockPrice = 21;
        double strikePrice = 20;
        double risklessRate = 0.1;
        double duration = 0.25;
        double optionValue = 1.875;
        
        double result = 0.23449;   // Value is obtained from example given in chapter
                                   // 14 of Options, Future, and Derivatives 8th edition
        
        
        // Keep track of performance to analyze vs. other methods of finding implied vol
        auto start = std::chrono::system_clock::now();
        double impliedVol = BlackScholesMerton::valueImpliedVolatilityNewtonRaphson(func, stockPrice, strikePrice, risklessRate, duration, optionValue);
        auto end = std::chrono::system_clock::now();
        
        //REQUIRE(Utilities::closeEnough(impliedVol, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // convert manually because if we casted in milliseconds we would get 0 by rounding
        std::cout << "Newton-Raphson method implied vol took " <<  elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds.\n";
    }
    
    SECTION("Implied volatility on European call option: Newton-Raphson method")
    {
        auto func = BlackScholesMerton::valueCallOption;
        
        double stockPrice = 100;
        double strikePrice = 100;
        double risklessRate = 0.05;
        double duration = 1.0;
        double optionValue = 10.5;
        
        double result = 0.201314;   // Value is obtained from QuantStart
                                    // http://www.quantstart.com/articles/Implied-Volatility-in-C-using-Template-Functions-and-Interval-Bisection
        
        
        // Keep track of performance to analyze vs. other methods of finding implied vol
        auto start = std::chrono::system_clock::now();
        double impliedVol = BlackScholesMerton::valueImpliedVolatilityNewtonRaphson(func, stockPrice, strikePrice, risklessRate, duration, optionValue);
        auto end = std::chrono::system_clock::now();
        
        REQUIRE(Utilities::closeEnough(impliedVol, result));
        
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        // convert manually because if we casted in milliseconds we would get 0 by rounding
        std::cout << "Newton-Raphson method implied vol took " <<  elapsed.count() / Utilities::NANO_TO_MILLI << " milliseconds.\n";
    }
    
    SECTION("D1 helper method of Black-Scholes-Merton formula")
    {
        double stockPrice = 42.0;
        double strikePrice = 40.0;
        double risklessRate = 0.1;
        double vol = 0.2;
        double duration = 0.5;
        
        double result = 0.7693; // Value is obtained from example given in chapter
                                // 14 of Options, Future, and Derivatives 8th edition
        
        double d1 = BlackScholesMerton::calcD1(stockPrice, strikePrice, risklessRate, vol, duration);
        
        REQUIRE(Utilities::closeEnough(d1, result));
    }
    
    SECTION("D1 helper method of Black-Scholes-Merton formula")
    {
        double stockPrice = 49.0;
        double strikePrice = 50.0;
        double risklessRate = 0.05;
        double vol = 0.2;
        double duration = 0.3846;
        
        double result = 0.054173; // Value is obtained from example given in chapter 18
                                  // of Options, Future, and Derivatives 8th edition
        
        double d1 = BlackScholesMerton::calcD1(stockPrice, strikePrice, risklessRate, vol, duration);
        
        REQUIRE(Utilities::closeEnough(d1, result));
    }
    
    SECTION("Differentiate function y=x^2")
    {
        // y = x^2
        auto func = [&](double x) -> double { return x * x; };
        
        
        double res = Utilities::differentiate(func, 2);
        REQUIRE(Utilities::closeEnough(res, 4.0));
        
        res = Utilities::differentiate(func, 1);
        REQUIRE(Utilities::closeEnough(res, 2.0));
        
        res = Utilities::differentiate(func, -2);
        REQUIRE(Utilities::closeEnough(res, -4.0));
        
        res = Utilities::differentiate(func, 8);
        REQUIRE(Utilities::closeEnough(res, 16.0));
    }
}