#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculator.h"
#include <ctype.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


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
        if ( reciprocal == floor(reciprocal) && fmod(reciprocal,2.0) == 1.0) {
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

 double calculate_factorial(int n) {
    if(n < 0) {
        fprintf(stderr, "Error: factorial of a negative number is undefined.\n");
        return NAN;
    }
    if (n == 0 || n == 1) {
        return 1.0;
    }
    double result = 1.0;
    for(int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
 }




double calculate_trig(const char *function, double x) {
    char func_lower[10] = {0};  
    strncpy(func_lower, function, sizeof(func_lower) - 1);  
    for (int i = 0; func_lower[i]; i++) {
        func_lower[i] = tolower(func_lower[i]);
    }

    if (strcmp(func_lower, "sin") == 0) {
        return sin(x);
    }
    else if (strcmp(func_lower, "cos") == 0) {
        return cos(x);
    }
    else if (strcmp(func_lower, "tan") == 0) {
        if (fmod(x + M_PI/2, M_PI) == 0) {
            fprintf(stderr, "Error: tan(x) is undefined at x = %f (π/2 + kπ).\n", x);
            return NAN;
        }
        return tan(x);
    }

    else if (strcmp(func_lower, "asin") == 0 || strcmp(func_lower, "arcsin") == 0) {
        if (x < -1 || x > 1) {
            fprintf(stderr, "Error: arcsin(x) requires x ∈ [-1, 1]. Got x = %f.\n", x);
            return NAN;
        }
        return asin(x);
    }
    else if (strcmp(func_lower, "acos") == 0 || strcmp(func_lower, "arccos") == 0) {
        if (x < -1 || x > 1) {
            fprintf(stderr, "Error: arccos(x) requires x ∈ [-1, 1]. Got x = %f.\n", x);
            return NAN;
        }
        return acos(x);
    }
    else if (strcmp(func_lower, "atan") == 0 || strcmp(func_lower, "arctan") == 0) {
        return atan(x);
    }

    else if (strcmp(func_lower, "sinh") == 0) {
        return sinh(x);
    }
    else if (strcmp(func_lower, "cosh") == 0) {
        return cosh(x);
    }
    else if (strcmp(func_lower, "tanh") == 0) {
        return tanh(x);
    }

    else if (strcmp(func_lower, "asinh") == 0 || strcmp(func_lower, "arsinh") == 0) {
        return asinh(x);  
    }
    else if (strcmp(func_lower, "acosh") == 0 || strcmp(func_lower, "arcosh") == 0) {
        if (x < 1.0) {
            fprintf(stderr, "Error: arcosh(x) requires x ≥ 1. Got x = %f.\n", x);
            return NAN;
        }
        return acosh(x);
    }
    else if (strcmp(func_lower, "atanh") == 0 || strcmp(func_lower, "artanh") == 0) {
        if (x <= -1.0 || x >= 1.0) {
            fprintf(stderr, "Error: artanh(x) requires -1 < x < 1. Got x = %f.\n", x);
            return NAN;
        }
        return atanh(x);
    }

    else if (strcmp(func_lower, "csc") == 0 || strcmp(func_lower, "cosec") == 0) {
        double sin_x = sin(x);
        if (sin_x == 0) {
            fprintf(stderr, "Error: csc(x) is undefined where sin(x) = 0 (x = kπ).\n");
            return NAN;
        }
        return 1.0 / sin_x;
    }
    else if (strcmp(func_lower, "sec") == 0) {
        double cos_x = cos(x);
        if (cos_x == 0) {
            fprintf(stderr, "Error: sec(x) is undefined where cos(x) = 0 (x = π/2 + kπ).\n");
            return NAN;
        }
        return 1.0 / cos_x;
    }
    else if (strcmp(func_lower, "cot") == 0 || strcmp(func_lower, "cotan") == 0) {
        double tan_x = tan(x);
        if (tan_x == 0) {
            fprintf(stderr, "Error: cot(x) is undefined where tan(x) = 0 (x = kπ).\n");
            return NAN;
        }
        return 1.0 / tan_x;
    }

    fprintf(stderr, "Error: Unknown function '%s'.\n", function);
    return NAN;
}