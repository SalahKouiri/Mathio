#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PARENTHESIS, 
    TOKEN_FUNCTION, 
}TokenType;

typedef struct {
    TokenType type;
    char value[32];
} Token;

#endif