#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "token.h"
#include "utils/utils.h"
#include "parser.h"
#include "calculator.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif

const char* VALID_FUNCTIONS[] = {
    "sin", "cos", "tan",
    "asin", "arcsin",
    "acos", "arccos",
    "atan", "arctan",
    "sinh", "cosh", "tanh",
    "asinh", "acosh", "atanh",
    "log", "sqrt", "power",
    NULL  
};

int is_valid_function(const char* name) {
    for(int i = 0; VALID_FUNCTIONS[i] != NULL; i++) {
        if(strcasecmp(name, VALID_FUNCTIONS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

double get_constant_value(const char *name) {
    if (strcasecmp(name, "pi") == 0) {
        return M_PI;
    }
    else if (strcasecmp(name, "e") == 0) {
        return M_E;
    }
    return NAN;
}

void remove_spaces(const char *input, char *output) {
    size_t j = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (input[i] != ' ') {
            output[j++] = input[i];
        }
    }
    output[j] = '\0'; 
}

void insert_implicit_multiplication(Token **tokens, int *token_count, int *capacity) {
    for (int i = 0; i < *token_count - 1; i++) {
        int need_multiply = 0;
        
        if ((*tokens)[i].type == TOKEN_NUMBER) {
            if ((*tokens)[i + 1].type == TOKEN_NUMBER) {
                need_multiply = 1;
            }
            else if ((*tokens)[i + 1].type == TOKEN_FUNCTION || 
                    (*tokens)[i + 1].type == TOKEN_VARIABLE ||
                    ((*tokens)[i + 1].type == TOKEN_PARENTHESIS && (*tokens)[i + 1].value[0] == '(')) {
                need_multiply = 1;
            }
        }
        else if ((*tokens)[i].type == TOKEN_VARIABLE || 
                ((*tokens)[i].type == TOKEN_PARENTHESIS && (*tokens)[i].value[0] == ')')) {
            
            if ((*tokens)[i + 1].type == TOKEN_FUNCTION || 
                (*tokens)[i + 1].type == TOKEN_VARIABLE ||
                ((*tokens)[i + 1].type == TOKEN_PARENTHESIS && (*tokens)[i + 1].value[0] == '(')) {
                need_multiply = 1;
            }
        }

        if (need_multiply) {
            if (*token_count >= *capacity) {
                *capacity *= 2;
                *tokens = realloc(*tokens, *capacity * sizeof(Token));
                if (*tokens == NULL) {
                    fprintf(stderr, "Error: Memory allocation failed during multiplication insertion.\n");
                    exit(EXIT_FAILURE);
                }
            }

            for (int j = *token_count; j > i + 1; j--) {
                (*tokens)[j] = (*tokens)[j - 1];
            }

            (*tokens)[i + 1].type = TOKEN_OPERATOR;
            (*tokens)[i + 1].value[0] = '*';
            (*tokens)[i + 1].value[1] = '\0';
            
            (*token_count)++;
            i++; 
        }
    }
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

        if (isalpha(cleaned_expression[i])) {
            int start = i;
            while (isalpha(cleaned_expression[i])) {
                i++;
            }
            
            strncpy((*tokens)[*token_count].value, &cleaned_expression[start], i - start);
            (*tokens)[*token_count].value[i - start] = '\0';
            
            if (strcasecmp((*tokens)[*token_count].value, "pi") == 0) {
                snprintf((*tokens)[*token_count].value, sizeof((*tokens)[*token_count].value), 
                        "%g", M_PI);
                (*tokens)[*token_count].type = TOKEN_NUMBER;
            } else if (strcasecmp((*tokens)[*token_count].value, "e") == 0) {
                snprintf((*tokens)[*token_count].value, sizeof((*tokens)[*token_count].value), 
                        "%g", M_E);
                (*tokens)[*token_count].type = TOKEN_NUMBER;
            } else if (strlen((*tokens)[*token_count].value) == 1) {
                (*tokens)[*token_count].type = TOKEN_VARIABLE;
            } else if (is_valid_function((*tokens)[*token_count].value)) {
                (*tokens)[*token_count].type = TOKEN_FUNCTION;
            } else {
                printf("Error: Unknown identifier '%s'.\n", (*tokens)[*token_count].value);
                free(cleaned_expression);
                free(*tokens);
                return -1;
            }
            
            (*token_count)++;
            continue;
        }

        if (isdigit(cleaned_expression[i]) || 
            (cleaned_expression[i] == '-' && 
             (i == 0 || cleaned_expression[i - 1] == '(' || strchr("+-*/^,", cleaned_expression[i - 1])))) {
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

        if (cleaned_expression[i] == '!') {
            (*tokens)[*token_count].value[0] = '!';
            (*tokens)[*token_count].value[1] = '\0';
            (*tokens)[*token_count].type = TOKEN_FACTORIAL;
            (*token_count)++;
            i++;
            continue;
        }

        if (cleaned_expression[i] == ',') {
            (*tokens)[*token_count].value[0] = ',';
            (*tokens)[*token_count].value[1] = '\0';
            (*tokens)[*token_count].type = TOKEN_COMMA;
            (*token_count)++;
            i++;
            continue;
        }

        if (strchr("+-*/^%", cleaned_expression[i])) {
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

    insert_implicit_multiplication(tokens, token_count, &capacity);

    free(cleaned_expression); 
    return 0; 
}

// Define operator precedence
int get_operator_precedence(char op) {
    switch (op) {
        case '^': return 4;
        case '*': case '/': case '%': return 3;
        case '+': case '-': return 2;
        default: return 0;
    }
}

// Check if token is a function
int is_function_token(TokenType type) {
    return type == TOKEN_FUNCTION;
}

// Convert infix expression to RPN (postfix)
int infix_to_postfix(Token *infix, int infix_count, Token **postfix, int *postfix_count) {
    // Allocate memory for postfix expression (max size = infix size)
    *postfix = safe_malloc(infix_count * sizeof(Token));
    *postfix_count = 0;
    
    // Operator stack
    Token *op_stack = safe_malloc(infix_count * sizeof(Token));
    int stack_size = 0;
    
    // Process each token in the infix expression
    for (int i = 0; i < infix_count; i++) {
        switch (infix[i].type) {
            case TOKEN_NUMBER:
            case TOKEN_VARIABLE:
                // Numbers and variables go directly to output
                (*postfix)[(*postfix_count)++] = infix[i];
                break;
                
            case TOKEN_FUNCTION:
                // Functions go on the stack
                op_stack[stack_size++] = infix[i];
                break;
                
            case TOKEN_OPERATOR:
                // Handle operators according to precedence
                while (stack_size > 0 &&
                       op_stack[stack_size-1].type == TOKEN_OPERATOR &&
                       get_operator_precedence(op_stack[stack_size-1].value[0]) >= 
                       get_operator_precedence(infix[i].value[0])) {
                    (*postfix)[(*postfix_count)++] = op_stack[--stack_size];
                }
                op_stack[stack_size++] = infix[i];
                break;
                
            case TOKEN_PARENTHESIS:
                if (infix[i].value[0] == '(') {
                    // Left parenthesis goes on the stack
                    op_stack[stack_size++] = infix[i];
                } else {
                    // Right parenthesis: pop until matching left parenthesis
                    while (stack_size > 0 && 
                           !(op_stack[stack_size-1].type == TOKEN_PARENTHESIS && 
                             op_stack[stack_size-1].value[0] == '(')) {
                        (*postfix)[(*postfix_count)++] = op_stack[--stack_size];
                    }
                    
                    // Discard the left parenthesis
                    if (stack_size > 0) stack_size--;
                    
                    // If the top of the stack is a function, pop it
                    if (stack_size > 0 && is_function_token(op_stack[stack_size-1].type)) {
                        (*postfix)[(*postfix_count)++] = op_stack[--stack_size];
                    }
                }
                break;
                
            case TOKEN_COMMA:
                // Pop operators until left parenthesis
                while (stack_size > 0 && 
                       !(op_stack[stack_size-1].type == TOKEN_PARENTHESIS && 
                         op_stack[stack_size-1].value[0] == '(')) {
                    (*postfix)[(*postfix_count)++] = op_stack[--stack_size];
                }
                break;
                
            case TOKEN_FACTORIAL:
                // Factorial is a unary operator that goes directly to output 
                // (it's applied to the last value)
                (*postfix)[(*postfix_count)++] = infix[i];
                break;
                
            default:
                // Other token types (handle as needed)
                break;
        }
    }
    
    // Pop any remaining operators from the stack
    while (stack_size > 0) {
        (*postfix)[(*postfix_count)++] = op_stack[--stack_size];
    }
    
    free(op_stack);
    return 0;
}

// Get number of arguments for a function
int get_function_arg_count(const char *function_name) {
    if (strcasecmp(function_name, "sin") == 0 ||
        strcasecmp(function_name, "cos") == 0 ||
        strcasecmp(function_name, "tan") == 0 ||
        strcasecmp(function_name, "sqrt") == 0 ||
        strcasecmp(function_name, "asin") == 0 || 
        strcasecmp(function_name, "arcsin") == 0 ||
        strcasecmp(function_name, "acos") == 0 || 
        strcasecmp(function_name, "arccos") == 0 ||
        strcasecmp(function_name, "atan") == 0 || 
        strcasecmp(function_name, "arctan") == 0 ||
        strcasecmp(function_name, "sinh") == 0 ||
        strcasecmp(function_name, "cosh") == 0 ||
        strcasecmp(function_name, "tanh") == 0 ||
        strcasecmp(function_name, "asinh") == 0 ||
        strcasecmp(function_name, "acosh") == 0 ||
        strcasecmp(function_name, "atanh") == 0) {
        return 1; // One argument functions
    } else if (strcasecmp(function_name, "log") == 0 ||
              strcasecmp(function_name, "power") == 0) {
        return 2; // Two argument functions
    }
    
    fprintf(stderr, "Error: Unknown function '%s'.\n", function_name);
    return 0; // Unknown function
}

// Apply a function to its arguments
double apply_function(const char *function_name, double *args, int arg_count) {
    if (strcasecmp(function_name, "sin") == 0 ||
        strcasecmp(function_name, "cos") == 0 ||
        strcasecmp(function_name, "tan") == 0 ||
        strcasecmp(function_name, "asin") == 0 || 
        strcasecmp(function_name, "arcsin") == 0 ||
        strcasecmp(function_name, "acos") == 0 || 
        strcasecmp(function_name, "arccos") == 0 ||
        strcasecmp(function_name, "atan") == 0 || 
        strcasecmp(function_name, "arctan") == 0 ||
        strcasecmp(function_name, "sinh") == 0 ||
        strcasecmp(function_name, "cosh") == 0 ||
        strcasecmp(function_name, "tanh") == 0 ||
        strcasecmp(function_name, "asinh") == 0 ||
        strcasecmp(function_name, "acosh") == 0 ||
        strcasecmp(function_name, "atanh") == 0) {
        if (arg_count != 1) {
            fprintf(stderr, "Error: Function '%s' requires 1 argument.\n", function_name);
            return NAN;
        }
        return calculate_trig(function_name, args[0]);
    } else if (strcasecmp(function_name, "sqrt") == 0) {
        if (arg_count != 1) {
            fprintf(stderr, "Error: Function 'sqrt' requires 1 argument.\n");
            return NAN;
        }
        return calculate_sqrt(args[0]);
    } else if (strcasecmp(function_name, "log") == 0) {
        if (arg_count != 2) {
            fprintf(stderr, "Error: Function 'log' requires 2 arguments.\n");
            return NAN;
        }
        return calculate_log(args[0], args[1]);
    } else if (strcasecmp(function_name, "power") == 0) {
        if (arg_count != 2) {
            fprintf(stderr, "Error: Function 'power' requires 2 arguments.\n");
            return NAN;
        }
        return calculate_power(args[0], args[1]);
    }
    
    fprintf(stderr, "Error: Unknown function '%s'.\n", function_name);
    return NAN;
}

// Evaluate a RPN expression
double evaluate_rpn(Token *postfix, int token_count) {
    #define MAX_STACK_SIZE 100
    #define MAX_FUNC_ARGS 10
    
    double value_stack[MAX_STACK_SIZE];
    int stack_size = 0;
    int arg_count = 0;
    
    for (int i = 0; i < token_count; i++) {
        switch (postfix[i].type) {
            case TOKEN_NUMBER:
                // Push number to stack
                value_stack[stack_size++] = atof(postfix[i].value);
                break;
                
            case TOKEN_OPERATOR:
                // Handle binary operators
                if (stack_size < 2) {
                    fprintf(stderr, "Error: Not enough values for operator '%s'.\n", postfix[i].value);
                    return NAN;
                }
                
                double op2 = value_stack[--stack_size]; // Second operand
                double op1 = value_stack[--stack_size]; // First operand
                double result;
                
                switch (postfix[i].value[0]) {
                    case '+': result = op1 + op2; break;
                    case '-': result = op1 - op2; break;
                    case '*': result = op1 * op2; break;
                    case '/': 
                        if (op2 == 0) {
                            fprintf(stderr, "Error: Division by zero.\n");
                            return NAN;
                        }
                        result = op1 / op2; 
                        break;
                    case '^': result = calculate_power(op1, op2); break;
                    case '%': 
                        if (op2 == 0) {
                            fprintf(stderr, "Error: Modulo by zero.\n");
                            return NAN;
                        }
                        result = fmod(op1, op2); 
                        break;
                    default:
                        fprintf(stderr, "Error: Unknown operator '%s'.\n", postfix[i].value);
                        return NAN;
                }
                
                value_stack[stack_size++] = result;
                break;
                
            case TOKEN_FUNCTION:
                // Handle functions
                arg_count = get_function_arg_count(postfix[i].value);
                
                if (stack_size < arg_count) {
                    fprintf(stderr, "Error: Not enough arguments for function '%s'.\n", postfix[i].value);
                    return NAN;
                }
                
                double args[MAX_FUNC_ARGS];
                
                // Pop arguments in reverse order
                for (int j = arg_count - 1; j >= 0; j--) {
                    args[j] = value_stack[--stack_size];
                }
                
                // Apply function
                double func_result = apply_function(postfix[i].value, args, arg_count);
                value_stack[stack_size++] = func_result;
                break;
                
            case TOKEN_FACTORIAL:
                // Factorial operator
                if (stack_size < 1) {
                    fprintf(stderr, "Error: Not enough values for factorial.\n");
                    return NAN;
                }
                
                double n = value_stack[--stack_size];
                double fact_result = calculate_factorial((int)n);
                value_stack[stack_size++] = fact_result;
                break;
                
            case TOKEN_VARIABLE:
                // For now, treat all variables as 0 (placeholder)
                fprintf(stderr, "Error: Variables not supported yet.\n");
                return NAN;
                
            default:
                // Other token types shouldn't appear in RPN
                fprintf(stderr, "Error: Invalid token in RPN expression.\n");
                return NAN;
        }
    }
    
    if (stack_size != 1) {
        fprintf(stderr, "Error: Invalid RPN expression (stack size = %d).\n", stack_size);
        return NAN;
    }
    
    return value_stack[0];  // Final result
}

// Main evaluation function that handles the whole process
double evaluate_expression(Token *tokens, int token_count) {
    Token *postfix;
    int postfix_count;
    
    // Convert infix to postfix
    if (infix_to_postfix(tokens, token_count, &postfix, &postfix_count) != 0) {
        fprintf(stderr, "Error converting expression to postfix notation.\n");
        return NAN;
    }
    
    // Evaluate postfix expression
    double result = evaluate_rpn(postfix, postfix_count);
    
    // Clean up
    free(postfix);
    
    return result;
}