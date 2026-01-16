#pragma once

#include "ping.h"

#include <stddef.h>

int ft_strlen(const char *string);
void zero_memory(void *ptr, size_t size);
void push_back_new_host(ip_t **head, char *hostname);
size_t host_list_size(ip_t *head);

int print_help();
int print_hint();
int print_err(const char *error, int return_value);
void print_struct(prog_t *prog);