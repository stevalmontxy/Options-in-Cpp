/*
Black scholes model based on model A in Appendix B of 
"Option Volatility and Pricing" by Sheldon Natenberg
This script calculates call and put prices based on static given inputs
the other script (not yet made) uses recent live data to calculate option price
last edited: 9/1/24
*/
#include <string>
#include <iostream>
#include <bits/stdc++.h> //include log() (natural log) and exp() (e^x) function
#include <cmath> // include sqrt() and abs() function

double calcH(double U, double E, double r, double v, double t);
double NprimeofX(double x);
double approxNofX(double x);
double calcCallPrice(double r, double t, double U, double E, double h, double v);


int main()
{
    double U{100}; // price of the underlying contract
    double E{97}; // exercise price
    double t{.3}; // time to expiration in years
    double v{.20}; // annual volatility expressed as a decimal fraction (sigma)
    double r{.04}; // risk-free interested rate expressed as a decimal fraction

    double h{};
    h = calcH(U, E, r, v, t);
    std::cout << "for a stock of price $" << U << ", exercise price $" << E << ", time of ~" << t*365 << " days:\n";
    std::cout.precision(2); // output things to 2 decimals
    std::cout << "the value of h is: " << h << '\n'; 
    std::cout << "the value of N'(h) is: " << NprimeofX(h) << '\n';
    std::cout << "the value of N(h) is: " << approxNofX(h) << '\n';
    std::cout << "the value of the call is: " << calcCallPrice(r, t, U, E, h, v) << '\n';


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
    // if (x >= 0.0) {
    //     double k {0};
    //     k = 1 / (1.0 + .33267 * abs(x));
    //     return 1.0 - NprimeofX(.4361836 * k - .1201676 * k*k + .9372980 * k*k*k);
    // }
    // else {
    // return 0.5;
    // }   
    return 1.0 /(1.0+exp(-1.65451*x));
} // https://mathoverflow.net/questions/19404/approximation-of-a-normal-distribution-function


double calcCallPrice(double r, double t, double U, double E, double h, double v)
{
    return U * approxNofX(h) - E * exp(-r*t) *approxNofX(h-v*sqrt(t));
}

