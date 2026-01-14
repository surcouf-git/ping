#include "utils.h"
#include "ping.h"
#include "parse_input.h"

#include <stdio.h>

int main(int argc, char **argv) {
	prog_t	prog;

	init(&prog);

	if (!parse_input(argc, argv, &prog))
		;//return (2);
	print_struct(&prog);
	return (0);
}