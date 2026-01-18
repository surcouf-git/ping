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
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

static struct sockaddr_in fill_client_infos(ip_t client) {
	struct sockaddr_in infos = {};

	infos.sin_family = AF_INET;
	infos.sin_addr.s_addr = htonl(client.decimal_ip);

	return (infos);
}

static int send_to_client(int socket, icmp_echo_t packet, ip_t client) {
	struct sockaddr_in client_infos = fill_client_infos(client);

	int bytes_sent = sendto(
				socket, 
				(void *)&packet, 
				sizeof(icmp_echo_t), 0,
				(struct sockaddr *)&client_infos, 
				sizeof(struct sockaddr_in)
			);

	if (bytes_sent == -1) {
		// TODO error
		fprintf(stderr, "Failed to send to client\n");
		return (NETWORKING_ERROR);
	}
	printf("Bytes sents: %i\n", bytes_sent);
	return (NETWORKING_SUCCESS);
}

static int send_packet(int socket, ip_t client) {
	icmp_echo_t packet = {};
	int packet_number = 0;

	while (packet_number < 3) { // TODO while true
		packet = build_echo_packet(0, packet_number);

		if (send_to_client(socket, packet, client) == NETWORKING_ERROR)
			return (NETWORKING_ERROR); // TODO 

		packet_number++;

	}
	return (PROCEED_NEXT_CLIENT);
}

/**
 * @todo clean doxygen here + need to see why the size sent is 40
 *       proceed errors, set good loop condition....
 */
int routine(prog_t *prog) {
	ip_t *clients = prog->ip_list;
	
	while (clients) {

		if (send_packet(prog->socket, *clients) == ROUTINE_NEED_STOP)
			break ; // TODO what to do ?

		clients = clients->next;
	}

	return (NETWORKING_SUCCESS);
}