#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/utils.h"
#include "token.h"
#include "parser.h"

int main() {
    printf("Welcome to Mathio: Testing Parser and Utils\n");
    printf("-------------------------------------------\n");

    char input[256];
    while (1) {
        printf("Enter a math expression (or 'exit' to quit): ");
        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = '\0';

        if (strncmp(input, "exit", 4) == 0) {
            break;
        }

        printf("\nTesting validate_expression...\n");
        if (!validate_expression(input)) {
            printf("Result: Invalid expression.\n");
            continue;
        }
        printf("Result: Valid expression.\n");

        printf("\nTesting tokenize...\n");
        Token *tokens = NULL;
        int token_count = 0;
        if (tokenize(input, &tokens, &token_count) < 0) {
            printf("Failed to tokenize the expression.\n");
            continue;
        }

        printf("Tokens:\n");
        for (int i = 0; i < token_count; i++) {
            printf("Type: %d, Value: %s\n", tokens[i].type, tokens[i].value);
        }

        free(tokens);
        printf("\n");
    }

    printf("Exiting Mathio. Goodbye!\n");
    return 0;
}