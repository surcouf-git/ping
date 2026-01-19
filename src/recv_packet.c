#include "networking.h"
#include "ping.h"

#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ip.h>

int receive_from_client(int socket, ip_t client, uint16_t identifier) {
	opti_t	packet = {};

	while (true) {
		int bytes = recvfrom(socket, (void *)&packet, sizeof(opti_t), 0, NULL, NULL);
		if (bytes == -1) {
			printf("error recv\n");
			continue ;
		}

		if (htons(packet.icmp_packet.identifier) == identifier) {
			printf("received from %u\n", ntohl(packet.ip_source));
			break ;
		}
		printf("ip source: %u | identifier: %u\n", ntohl(packet.ip_source), ntohs(packet.icmp_packet.identifier));
		sleep(1);
	}
}