#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

int is_valide_char(char c) {
    return isdigit(c) || strchr("+-*/^().!%, ",c) || isalpha(c);
}

int validate_expression(const char *expression) {
    if (expression == NULL || expression[0] == '\0') {
        printf("Error: Expression is empty.\n");
        return 0;
    }

    // Check for invalid starting operator (except minus for negative numbers)
    if (strchr("+-*/^%", expression[0]) && expression[0] != '-') {
        printf("Error: Expression cannot start with operator '%c'.\n", expression[0]);
        return 0;
    }

    int parentheses_count = 0;
    char last_non_space_char = '\0';

    for (size_t i = 0; expression[i] != '\0'; i++) {
        char c = expression[i];

        if (c == ' ') {
            continue;
        }

        if (!is_valide_char(c)) {
            printf("Error: Invalid character '%c' at position %zu.\n", c, i);
            return 0; 
        }

        if (c == '(') {
            parentheses_count++;
        } else if (c == ')') {
            parentheses_count--;
            if (parentheses_count < 0) {
                printf("Error: Unmatched closing parenthesis ')' at position %zu.\n", i);
                return 0;
            }
        } else if (c == '!') {
            if (!isdigit(last_non_space_char) && last_non_space_char != ')') {
                printf("Error: Factorial '!' must follow a number or ')'.\n");
                return 0;
            }
        } else if (c == '%') {
            if (!isdigit(last_non_space_char) && last_non_space_char != ')') {
                printf("Error: Modulo '%' must follow a number or ')'.\n");
                return 0;
            }
        } else if (c == ',') {
            if(!isdigit(last_non_space_char) && last_non_space_char != ')') {
                printf("Error: Comma ',' must follow a number or ')'.\n");
                return 0;
            }
        }

        last_non_space_char = c;
    }

    if (parentheses_count != 0) {
        printf("Error: Unmatched opening parenthesis '('.\n");
        return 0; 
    }

    if (strchr("+-*/^%,", last_non_space_char)) { 
        printf("Error: Expression ends with an operator '%c'.\n", last_non_space_char);
        return 0; 
    }

    return 1; 
}

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}