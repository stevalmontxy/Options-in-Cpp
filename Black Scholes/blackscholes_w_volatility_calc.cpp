/* Black scholes model based on model A in Appendix B of 
"Option Volatility and Pricing" by Sheldon Natenberg
This script calculates call and put prices based on inputs and historic stock close prices.
Another script (on the way) uses recent live data to calculate volatility
last edited: 9/7/24 */
#include <string>
#include <iostream>
#include <bits/stdc++.h> //include log() (natural log) and exp() (e^x) function
//since log is not constexpr, I can't make it all constexpr :(
#include <cmath> // include sqrt() and abs() function
#include <vector> // include vector type


double calcVolatility(std::vector<double> closePrice);
double calcH(double U, double E, double r, double v, double t);
double NprimeofX(double x);
double approxNofX(double x);
double calcCallPrice(double r, double t, double U, double E, double h, double v);
double calcPutPrice(double r, double t, double U, double E, double h, double v);


int main()
{
    std::vector<double> closePrice{548.99, 554.65, 553.78, 541.23, 538.41, 544.44, 544.76, 
            542.0, 550.81, 543.01, 532.9, 517.38, 522.15, 518.66, 530.65, 532.99, 533.27,
            542.04, 543.75, 553.07, 554.31, 559.61, 558.7, 560.62, 556.22, 562.13, 560.79, 
            561.56, 558.3, 558.35, 563.68, 552.08, 550.95, 549.61, 540.36}; // SPY daily adjusted close 7/19/2024 - 9/6/2024

    const double U{closePrice.back()}; // latest price of the underlying contract
    const double E{535}; // exercise price
    const double t{20 / 365.0}; // time to expiration in years (number  of days including weekends/365)
    const double r{.04}; // risk-free interested rate expressed as a decimal fraction

    const double v{calcVolatility(closePrice)}; // annual volatility expressed as a decimal fraction (sigma)
    const double h{calcH(U, E, r, v, t)};

    std::cout << "For a stock of price $" << U << ", exercise price $" << E << ", time of ~" << t*365 << " days to expiration:\n";
    std::cout << std::setprecision(2) << std::fixed; // output doubles to 2 decimals
    // std::cout << "The value of h is: " << h << '\n'; 
    // std::cout << "The value of N'(h) is: " << NprimeofX(h) << '\n';
    // std::cout << "The value of N(h) is: " << approxNofX(h) << '\n';
    std::cout << "The value of the call is: $" << calcCallPrice(r, t, U, E, h, v) << ".\n";
    std::cout << "The value of the put is: $" << calcPutPrice(r, t, U, E, h, v) << ".\n";

    return 0;
}

double calcVolatility(std::vector<double> closePrice)
{
    double vectorLength {closePrice.size()};

    std::vector<double> logPriceChanges;
        for (int i = 1; i < vectorLength; i++){
        logPriceChanges.push_back(log(closePrice[i]/closePrice[i-1]));
    }    
    
    double averageLogReturn = std::accumulate(logPriceChanges.begin(), logPriceChanges.end(), 0.0) / (vectorLength - 1);

    std::vector<double> logPriceDevFromMeanSquared;
    for (int i = 0; i < vectorLength-1; i++){
        logPriceDevFromMeanSquared.push_back(pow(logPriceChanges[i] - averageLogReturn,2));
    }

    double stdev {sqrt(std::accumulate(logPriceDevFromMeanSquared.begin(), logPriceDevFromMeanSquared.end(), 0.0)/(vectorLength -2))};
    double annualizedVolatility {stdev * sqrt(365)};

    std::cout << "Std deviation of log price changes: " << stdev << ", Annualized volatility: " << annualizedVolatility << '\n';
    return annualizedVolatility;
}

double calcH(double U, double E, double r, double v, double t)
{
    return (log(U/E) + (r + v*v/2) * t) / (v*sqrt(t));
}


double NprimeofX(double x)
{
    return exp(-x*x/2.0) / (sqrt(2.0 * M_PI));
}


double approxNofX(double x)
{
    return 1.0 /(1.0+exp(-1.65451*x));
} // https://mathoverflow.net/questions/19404/approximation-of-a-normal-distribution-function


double calcCallPrice(double r, double t, double U, double E, double h, double v)
{
    return U * approxNofX(h) - E * exp(-r*t) *approxNofX(h-v*sqrt(t));
}

double calcPutPrice(double r, double t, double U, double E, double h, double v)
{
    return -U * approxNofX(-h) + E * exp(-r*t) *approxNofX(v*sqrt(t)-h);
}
