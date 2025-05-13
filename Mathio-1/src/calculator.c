#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculator.h"

double calculate_sqrt(double x) {
    if (x < 0) {
        fprintf(stderr, "Error: Square root of negative number.\n");
    }

 double calculate_sqrt(double x) {
    if(x < 0 ) {
        fprintf(stderr, "Error: Square root of negative number.\n");
        return NAN;
    }
    return sqrt(x);
 }   

 double calculate_power(double base, double exponent) {
    if (base < 0 && exponent !=floor(exponent)) {
        double reciprocal = 1.0 / exponent ; 
        if ( repciprocal == floor(reciprocal) && fmod(reciprocal,2.0) == 1.0) {
            return pow( base , exponent );
        } else {
            fprintf(stderr, "Error: Negative base with non-integer exponent.\n");
            return NAN;
        }
    }
    return pow(base, exponent);
 }

 double calculate_log(double x, double base) {
    if (x <= 0 ) {
        fprintf(stderr, "Error: Logarithm of non-positive number.\n");
        return NAN;
    }
    if( base <= 0 || base == 1 ) {
        fprintf(stderr, "Error: Invalid logarithm base.\n");
        return NAN;
    }
    return log(x) / log(base);
 }

 double caculate_factorial(int n ) {
    if(n < 0) {
        fprintf(stderr,"Error: factorial of a negative number is undefined  .\n")
    }

 }





}