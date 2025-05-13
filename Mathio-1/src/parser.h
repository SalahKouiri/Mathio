#ifndef PARSER_H
#define PARSER_H

#include "token.h"

int tokenize(const char *expression, Token **tokens, int *token_count);

#endif 