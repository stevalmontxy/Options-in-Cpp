/*
Black scholes model based on model A in Appendix B of 
"Option Volatility and Pricing" by Sheldon Natenberg
This script calculates call and put prices based on static given inputs
the other script (not yet made) uses recent live data to calculate option price
last edited: 9/7/24
*/
#include <string>
#include <iostream>
#include <bits/stdc++.h> //include log() (natural log) and exp() (e^x) function
//since log is not constexpr, I can't make it all constexpr :(
#include <cmath> // include sqrt() and abs() function

double calcH(double U, double E, double r, double v, double t);
double NprimeofX(double x);
double approxNofX(double x);
double calcCallPrice(double r, double t, double U, double E, double h, double v);
double calcPutPrice(double r, double t, double U, double E, double h, double v);

int main()
{
    const double U{100}; // price of the underlying contract
    const double E{97}; // exercise price
    const double t{60.0 / 365}; // time to expiration in years (number  of days including weekends/365)
    const double v{.20}; // annual volatility expressed as a decimal fraction (sigma)
    const double r{.04}; // risk-free interested rate expressed as a decimal fraction

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
