#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

int is_valide_char(char c);

int validate_expression(const char *expression);

void *safe_malloc(size_t size);


#endif