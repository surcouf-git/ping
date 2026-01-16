#include "parse_input.h"
#include "messages.h"
#include "utils.h"
#include "ping.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

int check_argument_nb(int argc) {
	return (argc == 1 ? print_err(EALONE, 2) : 1);
}

struct option *init_opt(void) {
	static struct option
		long_options[] = {
			{"help", no_argument, 0, '?'},
			{"verbose", no_argument, 0, 'v'},
			{"version", no_argument, 0, 'V'},
			{0, 0, 0, 0}
		};
	return(long_options);
}

int handle_question_mark(char **argv) {
	if (optopt == 0) return (print_help());

	fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], optopt);
	print_hint();
	return (PARSE_ERROR);
}

/**
 * @brief	save in param 'prog' the given host 
 * 
 * @param[in] optind	variable given by getopt_long() to know where start arguments while option sort is done
 */
static int save_given_hosts(int optind, int argc, char **argv, prog_t *prog) {
	ip_t	*head = NULL;

	while (optind < argc) {
		push_back_new_host(&head, argv[optind]);
		optind++;
	}

	prog->ip_list = head;

	return (PARSE_OK);
}

/**
 * @brief	parse program argument line and init option structure list 
 *
 * @param[in] argc		argument line count
 * @param[in] argv		argument array
 * @param[out] prog		structure wich contains the whole program informations
 *
 * @return	PARSE_OK (1) on success PARSE_ERROR (0) on failure
 */
int parse_input(int argc, char **argv, prog_t *prog) {
	int 			opt = 0;
	struct option	*long_options = init_opt();

	// @var opterr	set this param to 0 tells getopt_long() to not display any info on the standard output
	opterr = 0;
	while ((opt = getopt_long(argc, argv, "Vv?", long_options, NULL)) != -1) {
		switch (opt) {
			case ('?'):
				return (handle_question_mark(argv));
			case ('v'):
				prog->opts.verbose = ENABLE_OPT;
				break ;
		}
	}
	return (save_given_hosts(optind, argc, argv, prog));
}