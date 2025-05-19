// parser.h
#ifndef PARSER_H
#define PARSER_H

#include "token.h"

// Function declarations
int tokenize(const char *expression, Token **tokens, int *token_count);
int get_operator_precedence(char op);
int is_function_token(TokenType type);
int infix_to_postfix(Token *infix, int infix_count, Token **postfix, int *postfix_count);
double evaluate_rpn(Token *postfix, int token_count);
int get_function_arg_count(const char *function_name);
double apply_function(const char *function_name, double *args, int arg_count);
double evaluate_expression(Token *tokens, int token_count); // Declare here


#endif // PARSER_H

