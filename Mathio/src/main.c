#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils/utils.h"
#include "token.h"
#include "parser.h"
#include "calculator.h"

// Declare the function here
double evaluate_expression(Token *tokens, int token_count);
void perform_calculation() {
    char input[256];
    while (1) {
        printf("\n> ");
        if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

            Token *tokens = NULL;
            int token_count = 0;
            if (tokenize(input, &tokens, &token_count) < 0) {
                printf("Tokenization failed.\n");
                free(tokens); // Free allocated memory in case of failure
                continue;
    }

            double result = evaluate_expression(tokens, token_count);
            if (!isnan(result)) {
                printf("Result: %.6f\n", result);
            } else {
                printf("Evaluation failed.\n");
}

            // Free the allocated memory for tokens after use
            free(tokens);
}
}
}

int main() {
    printf("Welcome to the Mathio Calculator\n");
    printf("===============================\n");
    perform_calculation();
    return 0;
}


