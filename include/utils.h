#pragma once

#include "ping.h"

#include <stddef.h>

int ft_strlen(const char *string);
void zero_memory(void *ptr, size_t size);
void push_back_new_host(ip_t **head, char *hostname);
size_t host_list_size(ip_t *head);
const char *ascii_addr_from_decimal(int decimal_addr, char *buffer);

void print_header(ip_t *client, opt_t opts, uint16_t identifier);
void print_stats(void);

int print_help();
int print_hint();
int print_err(const char *error, int return_value);
void print_struct(prog_t *prog);
void print_verbose_unreachable(opt_t *options, icmp_error_packet_t *packet);

void clean_packet(icmp_echo_t *packet);