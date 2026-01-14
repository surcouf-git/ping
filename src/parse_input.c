#include "parse_input.h"
#include "messages.h"
#include "utils.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

int check_argument_nb(int argc) {
	return (argc == 1 ? print_err(EALONE, 2) : 1);
}

struct option *init_opt(void) {
	static struct option
	long_options[] = {
		{NULL, no_argument, 0, '?'},
		{"verbose", no_argument, 0, 'v'},
		{NULL, no_argument, 0, 'V'},
		{0, 0, 0, 0}
	};
	return(long_options);
}

int parse_input(int argc, char **argv, prog_t *prog) {

	int opt = 0;

	struct option *long_options = init_opt();

	while ((opt = getopt_long(argc, argv, "Vv?", long_options, NULL))) {
		switch (opt) {
			case ('?'):
				prog->opts.help = ENABLED;
				break ;
			case ('v'):
				prog->opts.verbose = ENABLED;
				break ;
			case(UNKNOWN):
				return (print_hint());
		}
	}
	return (0);
}