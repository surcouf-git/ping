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
	printf("char verbose = %i;\n", prog->opts.verbose);
	tabs(1);
	// == closing opts
	printf("}\n");
	tabs(1);
	printf("frame_t frame {}\n");
	// == closing prog
	printf("}\n");
	//tabs(2);
	return ;
}

void push_back_new_host(ip_t **head, char *hostname) {
	ip_t *new_node = malloc(sizeof(ip_t));

	zero_memory((void *)new_node, sizeof(new_node));
	new_node->raw_hostname = hostname;
	new_node->next = NULL;

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