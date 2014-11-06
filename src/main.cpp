//
//  main.cpp
//  QuantLib
//
//  Created by Chris Len on 10/25/14.
//  Copyright (c) 2014 Chris Len. All rights reserved.
//

#include <iostream>
#include "Black-Scholes-Merton.h"

using namespace std;

void testBlackScholesMerton();

int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Welcome to the financial modeling program!\n\n";
    
    testBlackScholesMerton();
    
    
    return 0;
}

void testBlackScholesMerton()
{
    cout << "Black-Scholes-Merton Option Pricing:\n";
    cout << "------------------------------------------------------\n";
    
    double stockPrice = 42.0;
    double strikePrice = 40.0;
    double risklessRate = 0.1;
    double vol = 0.2;
    double duration = 0.5;
    
    // Start time to evaluate performance
    auto start = std::chrono::system_clock::now();
    
    // Stack allocation usually faster than heap allocation
    
    double callOption = BlackScholesMerton::valueCallOption(stockPrice, strikePrice,
                                                            risklessRate, vol, duration);
    double putOption = BlackScholesMerton::valuePutOption(stockPrice, strikePrice,
                                                          risklessRate, vol, duration);
    // Done work
    auto end = std::chrono::system_clock::now();
    
    cout << "Initial stock price: " << stockPrice << endl;
    cout << "Strike price: " << strikePrice << endl;
    cout << "Riskless rate:" << risklessRate << endl;
    cout << "Volatility of underlying stock:" << vol << endl;
    cout << "Time until maturity: " << duration << endl;
    cout << endl;
    
    cout << "Non-dividend paying call option valued at: " << callOption << endl;
    cout << "Non-divident paying put option valued at: " << putOption << endl;
    
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    // convert manually because if we casted in milliseconds we would get 0 by rounding
    cout << "Valuation took " <<  elapsed.count() / 100000.0 << " milliseconds.\n";
    
    stockPrice = 40;
    vol = 0.3;
    risklessRate = 0.09;
    double dividend = 0.5;
    
    // Start time to evaulate performance
    start = std::chrono::system_clock::now();
    
    // Stack allocation usually faster than heap allocation
    vector<double> payment{1.0/6.0,5.0/12.0};
    
    double callOptionWithDividends = BlackScholesMerton::valueCallOption(stockPrice, strikePrice,
                                                                         risklessRate, vol, duration, dividend, payment);
    double putOptionWithDividends = BlackScholesMerton::valuePutOption(stockPrice, strikePrice,
                                                                       risklessRate, vol, duration, dividend, payment);
    
    // Done work
    end = std::chrono::system_clock::now();
    
    cout << "Initial stock price: " << stockPrice << endl;
    cout << "Strike price: " << strikePrice << endl;
    cout << "Riskless rate:" << risklessRate << endl;
    cout << "Volatility of underlying stock:" << vol << endl;
    cout << "Time until maturity: " << duration << endl;
    cout << endl;
    
    cout << "Dividend paying call option valued at: " << callOptionWithDividends << endl;
    cout << "Divident paying put option valued at: " << putOptionWithDividends << endl;
    
    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    // convert manually because if we casted in milliseconds we would get 0 by rounding
    cout << "Valuation took " <<  elapsed.count() / 100000.0 << " milliseconds.\n";
}