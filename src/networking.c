/**
 * @file networking.c
 * 
 * @brief extract the host(s) given via the program line argument and build an echo packet
 * 
 * @author surcouf-git
 */

#include "networking.h"
#include "ping.h"
#include "utils.h"
#include "messages.h"

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <unistd.h>


static int init_socket(prog_t *prog) {
	prog->socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (prog->socket == -1) {
		// TODO wich error ?
		fprintf(stderr, "Error while creating socket (errno:%d)! \n", errno);
		return (NETWORKING_ERROR);
	}
	return (NETWORKING_SUCCESS);
}

/**
 * @brief    first function (retreive_host_infos()) try to resolve given IP(s) 
 *           on program argument and store it into a list: prog->ip_list
 */
int init_networking(prog_t *prog) {
	if (retreive_host_infos(prog) == NO_HOST_GIVEN) return (NETWORKING_ERROR);

	if (init_socket(prog) == NETWORKING_ERROR) return (NETWORKING_ERROR);

	return (NETWORKING_SUCCESS);
}

struct sockaddr_in fill_client_infos(ip_t client) {
	struct sockaddr_in infos = {};

	infos.sin_family = AF_INET;
	infos.sin_addr.s_addr = htonl(client.decimal_ip);

	return (infos);
}

int ping(int socket, ip_t client, opt_t opts) {
	icmp_echo_t packet = {};
	int packet_number = 0;
	uint16_t identifier = getpid();

	printf("identifier: %u\n", identifier);

	while (PACKET_COUNT) { // TODO while true

		packet = build_echo_packet(identifier, packet_number);

		if (send_to_client(socket, packet, client) == NETWORKING_ERROR)
			return (NETWORKING_ERROR); // TODO

			
		if (receive_from_client(socket, client, identifier) == NETWORKING_ERROR)
			return (NETWORKING_ERROR);
			
		packet_number++;
		sleep(1);
	}
	return (PROCEED_NEXT_CLIENT);
}

/**
 * @todo clean doxygen here + need to see why the size sent is 40
 *       proceed errors, set good loop condition....
 */
int routine(prog_t *prog) {
	ip_t *clients = prog->ip_list;
	
	printf("sizeof packet: %lu\n", sizeof(icmp_echo_t));
	while (clients) {

		if (ping(prog->socket, *clients, prog->opts) == ROUTINE_NEED_STOP)
			break ;

		clients = clients->next;

	}
	return (NETWORKING_SUCCESS);
}