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
#include <stdlib.h>
#include <sys/time.h>


static int create_socket(void) {
	int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock == -1) {
		// TODO wich error ?
		fprintf(stderr, "Error while creating socket (errno:%d)! \n", errno);
		return (NETWORKING_ERROR);
	}
	return (sock);
}

//static void set_socket_timeout(int socket, int timeout) {
//	struct timeval tv = {};
//	tv.tv_sec = timeout;
//	tv.tv_usec = 0;

//	setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
//}

static int init_socket(prog_t *prog) {
	prog->socket = create_socket();
	if (prog->socket == -1) return (NETWORKING_ERROR);

	//set_socket_timeout(prog->socket, 5);
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

int ping(int socket, ip_t client, opt_t opts, uint16_t identifier) {
	icmp_echo_t		*packet = NULL;
	size_t			data_size = opts.data_size;
	int				packet_number = 0;
	elapsed_time_t	timer = {};

	while (PACKET_COUNT) { // TODO while true

		packet = build_echo_packet(identifier, packet_number, data_size);

		if (send_to_client(socket, packet, client, data_size, &timer) == NETWORKING_ERROR)
			return (NETWORKING_ERROR); // TODO

		if (receive_from_client(socket, opts, identifier, data_size, &timer) == NETWORKING_ERROR)
			return (NETWORKING_ERROR);
			
		packet_number++;
		sleep(1);

		clean_packet(packet);
	}
	return (PROCEED_NEXT_CLIENT);
}

/**
 * @todo clean doxygen here + need to see why the size sent is 40
 *       proceed errors, set good loop condition....
 */
int routine(prog_t *prog) {
	ip_t *client = prog->ip_list;
	
	while (client) {

		uint16_t identifier = getpid();

		print_header(client, prog->opts, identifier);

		if (ping(prog->socket, *client, prog->opts, identifier) == ROUTINE_NEED_STOP)
			break ;

		print_stats();

		client = client->next;
	}
	return (NETWORKING_SUCCESS);
}