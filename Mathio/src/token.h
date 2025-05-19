#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PARENTHESIS, 
    TOKEN_FUNCTION, 
    TOKEN_FACTORIAL,  
    TOKEN_MODULO,     
    TOKEN_CONSTANT,  
    TOKEN_VARIABLE,  
    TOKEN_COMMA, // New token type for variables
}TokenType;

typedef struct {
    TokenType type;
    char value[32];
} Token;

#endif