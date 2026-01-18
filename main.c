/**
 * 
 * @ref networking.c
 *
 * @author surcouf-git
 */

#include "utils.h"
#include "ping.h"
#include "parse_input.h"
#include "networking.h"

#include <stdio.h>
#include <stdlib.h>

static void clean_up(prog_t *prog) {
	ip_t *current = prog->ip_list, *next = NULL;
	while (current) {
		next = current->next;
		free(current);
		current = next;
	}
}

/**
 * @brief     entry point
 * 
 * @public
 */
static int run(int argc, char **argv, prog_t *prog) {
	if (parse_input(argc, argv, prog) == PARSE_ERROR)
		return (EXIT_FAILURE);

	if (init_networking(prog) == NETWORKING_ERROR)
		return (EXIT_FAILURE);

	if (send_packet(prog) == NETWORKING_ERROR)
		return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}

int main(int argc, char **argv) {
	prog_t	prog = {};

	run(argc, argv, &prog); // TODO return ?

	print_struct(&prog);

	clean_up(&prog);
	return (0);
}