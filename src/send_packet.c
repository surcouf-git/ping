#include "networking.h"
#include "ping.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>


int send_to_client(int socket, icmp_echo_t packet, ip_t client) {
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
		perror("failed to send: ");
		fprintf(stderr, "Failed to send to client\n");
		
		return (NETWORKING_ERROR);
	}
	printf("Bytes sents: %i\n", bytes_sent);
	return (NETWORKING_SUCCESS);
}