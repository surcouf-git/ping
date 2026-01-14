#pragma once

#include "ping.h"

#include <stddef.h>

int ft_strlen(const char *string);
void zero_memory(char *ptr, size_t size);

int print_hint();
int print_err(const char *error, int return_value);
void print_struct(prog_t *prog);