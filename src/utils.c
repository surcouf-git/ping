#include "utils.h"
#include "messages.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>

int ft_strlen(const char *string) {
	if (!string)
		return (0);

	int i = 0;
	while (string[i]) i++;

	return (i);
}

int print_err(const char *error, int return_value) {
	fprintf(stderr, "%s\n", error);
	return (return_value);
}

int print_help() {
	return (0);
}

int print_hint() {
	printf("Try 'ft_ping --help' or 'ft_ping --usage' for more information.\n");
	return (2);
}

void zero_memory(void *ptr, size_t size) {
	char *lptr = (char *)ptr;

	for (size_t i = 0; i < size; i++) {
		lptr[i] = 0;
	}
}

static void tabs(int nb) {
	for (int i = 0; i < nb; i++) {
		printf("\t");
	}
}

void print_struct(prog_t *prog) {
	// == prog ==
	printf("prog_t prog {\n");
	// == opts ==
	tabs(1);
	printf("opt_t opts {\n");
	tabs(2);
	printf("int host_count = %d;\n", prog->opts.host_count);
	tabs(2);
	printf("char verbose = %i;\n", prog->opts.verbose);
	tabs(2);
	printf("int end_opt = %i;\n", prog->opts.end_opt);
	tabs(1);
	// == closing opts
	printf("}\n");
	// == ip list
	tabs(1);
	printf("ip_t ip_list {\n");
	ip_t *ptr = prog->ip_list;
	int count = 0;
	while (ptr) {
		tabs(2);
		printf("------------------------------------------------\n");
		tabs(2);
		printf("node: %i\n", count);
		tabs(2);
		printf("char *raw_hostname: %s\n", ptr->raw_hostname);
		tabs(2);
		printf("uint decimal_ip   : %u\n", ptr->decimal_ip);
		//tabs(2);
		ptr = ptr->next;
		count++;
	}
	// == closing ip list
	tabs(1);
	printf("}\n");
	// == closing prog
	printf("}\n");
	return ;
}

void push_back_new_host(ip_t **head, char *hostname) {
	ip_t *new_node = malloc(sizeof(ip_t));

	zero_memory((void *)new_node, sizeof(ip_t));
	new_node->raw_hostname = hostname;

	if (!(*head)) {
		*head = new_node;
		return ;
	}

	ip_t *actual = *head;
	while (actual && actual->next) {
		actual = actual->next;
	}

	actual->next = new_node;
}

size_t host_list_size(ip_t *head) {
	size_t i = 0;

	while (head) {
		head = head->next;
		i++;
	}
	return (i);
}