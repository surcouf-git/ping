#include "utils.h"
#include "messages.h"

#include <stdio.h>
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

int print_hint() {
	printf("Try 'ft_ping --help' or 'ft_ping --usage' for more information.\n");
	return (2);
}

void zero_memory(char *ptr, size_t size) {
	for (size_t i = 0; i < size; i++) {
		ptr[i] = '\0';
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
	printf("char help = %i;\n", prog->opts.help);
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