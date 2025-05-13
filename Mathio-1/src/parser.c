#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "utils/utils.h"

void remove_spaces(const char *input, char *output) {
    size_t j = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (input[i] != ' ') {
            output[j++] = input[i];
        }
    }
    output[j] = '\0'; 
}

int tokenize(const char *expression, Token **tokens, int *token_count) {
    int capacity = 64; 
    *token_count = 0;
    *tokens = safe_malloc(capacity * sizeof(Token));

    char *cleaned_expression = safe_malloc(strlen(expression) + 1);
    remove_spaces(expression, cleaned_expression);

    size_t i = 0;
    while (cleaned_expression[i] != '\0') {
        if (*token_count >= capacity) {
            capacity *= 2; 
            *tokens = realloc(*tokens, capacity * sizeof(Token));
            if (*tokens == NULL) {
                fprintf(stderr, "Error: Memory allocation failed during tokenization.\n");
                free(cleaned_expression);
                exit(EXIT_FAILURE);
            }
        }

        if (isdigit(cleaned_expression[i]) || 
            (cleaned_expression[i] == '-' && 
             (i == 0 || cleaned_expression[i - 1] == '(' || strchr("+-*/^", cleaned_expression[i - 1])))) {
            int start = i;

            if (cleaned_expression[i] == '-') {
                i++; 
            }

            while (isdigit(cleaned_expression[i]) || cleaned_expression[i] == '.') {
                i++;
            }

            strncpy((*tokens)[*token_count].value, &cleaned_expression[start], i - start);
            (*tokens)[*token_count].value[i - start] = '\0';
            (*tokens)[*token_count].type = TOKEN_NUMBER;
            (*token_count)++;
            continue;
        }

        if (strchr("+-*/^", cleaned_expression[i])) {
            (*tokens)[*token_count].value[0] = cleaned_expression[i];
            (*tokens)[*token_count].value[1] = '\0';
            (*tokens)[*token_count].type = TOKEN_OPERATOR;
            (*token_count)++;
            i++;
            continue;
        }

        if (strchr("()", cleaned_expression[i])) {
            (*tokens)[*token_count].value[0] = cleaned_expression[i];
            (*tokens)[*token_count].value[1] = '\0';
            (*tokens)[*token_count].type = TOKEN_PARENTHESIS;
            (*token_count)++;
            i++;
            continue;
        }

        printf("Error: Invalid character '%c' in expression.\n", cleaned_expression[i]);
        free(cleaned_expression);
        free(*tokens); 
        return -1;
    }

    free(cleaned_expression); 
    return 0; 
}