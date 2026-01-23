#include "networking.h"
#include "ping.h"
#include "utils.h"

#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>

double get_elapsed_time(struct timespec start_time, struct timespec end_time) {
	return ((end_time.tv_sec - start_time.tv_sec) * 1000.0 +
		(end_time.tv_nsec - start_time.tv_nsec) / 1e6);
}

static void print_received_packet(void *packet_raw, size_t data_size, elapsed_time_t *timer) {
	char				ip[INET_ADDRSTRLEN] = {};
	icmp_echo_reply_t	*packet = (icmp_echo_reply_t *)packet_raw;
	ssize_t				reply_size = sizeof(icmp_echo_t) + data_size;
	int					sequence = ntohs(packet->icmp_packet.sequence_number), ttl = packet->ttl;

	clock_gettime(CLOCK_MONOTONIC, &timer->end_time);

	double elapsed = get_elapsed_time(timer->start_time, timer->end_time);
		
	ascii_addr_from_decimal(ntohl(packet->ip_source), ip);
	printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", reply_size , ip, sequence, ttl, elapsed);
}

static bool is_waited_packet(void *packet_raw, uint16_t identifier) {
	icmp_echo_reply_t	*packet = (icmp_echo_reply_t *)packet_raw;

	if (ntohs(packet->icmp_packet.identifier) == identifier)
		return (true);
	return (false);
}

static void print_unreachable_error(const char *type, const char *from, ssize_t bytes_received) {
	printf("%lu bytes from %s: %s Unreachable\n", bytes_received - sizeof(struct iphdr), from, type);
}

static bool is_unreachable(void *packet_raw, opt_t options, uint16_t identifier, struct sockaddr_in *from, ssize_t bytes_received) {
	const char			*addr = inet_ntoa(from->sin_addr);
	icmp_error_packet_t	*error_packet = (icmp_error_packet_t *)packet_raw;

	if (ntohs(error_packet->original_icmp.identifier) != identifier)
		return (false);

	if (error_packet->icmp.type == ICMP_DEST_UNREACH) {
		switch (error_packet->icmp.code) {

			case (ICMP_NET_UNREACH):
				print_unreachable_error("Network", addr, bytes_received);
				break ;

			case (ICMP_HOST_UNREACH):
				print_unreachable_error("Destination Host", addr, bytes_received);
				break ;

		}
		print_verbose_unreachable(&options, error_packet);
		return (true);
	}
	return (false);
}

int receive_from_client(int socket, opt_t options, uint16_t identifier, size_t data_size, elapsed_time_t *timer) {
	char		packet_raw[MAX_PACKET_SIZE] = {};
	ssize_t		recv_bytes = 0;

	struct sockaddr_in client_infos = {};
	socklen_t len = sizeof(struct sockaddr_in);
	while (true) {

		recv_bytes = recvfrom(
						socket,
						packet_raw, MAX_PACKET_SIZE,
						0,
						(struct sockaddr *)&client_infos,
						&len
					); // @todo recv size 

		if (recv_bytes == -1)
			return (NETWORKING_ERROR);

		if (is_unreachable(packet_raw, options, identifier, &client_infos, recv_bytes) == true)
			return (NETWORKING_SUCCESS);

		if (is_waited_packet(packet_raw, identifier))
			break ;

	}
	print_received_packet(packet_raw, data_size, timer);
	return (NETWORKING_SUCCESS);
}