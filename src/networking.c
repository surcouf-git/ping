/**
 * @file networking.c
 * 
 * @brief extract the host(s) given via the program argument and build an echo packet
 * 
 * @author surcouf-git
 */

#include "networking.h"
#include "ping.h"
#include "utils.h"
#include "messages.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

static struct addrinfo *set_addrinfo_hints(void) {
	static struct addrinfo hint = {
						.ai_family = AF_INET,
						.ai_socktype = SOCK_RAW,
						.ai_protocol = IPPROTO_ICMP
					};
	return (&hint);
}

static int extract_ip(ip_t *host_ptr, struct addrinfo *hint) {
	struct addrinfo *response = NULL;

	if (getaddrinfo(host_ptr->raw_hostname, NULL, hint, &response) == 0) {
		struct sockaddr_in *addr = (struct sockaddr_in *)response->ai_addr;
		host_ptr->decimal_ip = addr->sin_addr.s_addr;
	} else {
		gai_strerror(errno); // TODO return what on error ?
	}
	freeaddrinfo(response);
	return (VALID_HOST);
}

static int retreive_host_infos(prog_t *prog) {
	struct addrinfo	*hint = set_addrinfo_hints();

	if (prog->opts.host_count == 0) {
		print_err(EALONE, 0);
		print_err(PHINT, 0); // TODO --usage ?? declenche l'erreur
		return (NO_HOST_GIVEN);
	}

	ip_t *host_ptr = prog->ip_list;
	while (host_ptr) {
		extract_ip(host_ptr, hint);
		host_ptr = host_ptr->next;
	}
	return (NETWORKING_SUCCESS);
}

static void set_type(icmp_echo_t *packet, uint8_t type) {
	packet->type = type;
}

static void set_code(icmp_echo_t *packet, uint8_t code) {
	packet->code = code;
}

static void set_identifier(icmp_echo_t *packet, uint16_t identifier) {
	packet->identifier = identifier;
}

static void set_sequence_number(icmp_echo_t *packet) {
	static int sequence = 1;
	packet->sequence_number = sequence;
}

uint16_t checksum(const void *data, size_t length) {
	const uint16_t *ptr = data;
	uint32_t sum = 0;

	while (length > 1) {
		sum += *ptr++;
		length -= 2;
	}

	if (length == 1)
		sum += *((const uint8_t *)ptr);

	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	return (uint16_t)(~sum);
}

static void set_data(icmp_echo_t *packet) {

	int fd = open("/dev/random", O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "cannot open /dev/random\n"); // @todo handle error ?
		return ;
	}

	if (read(fd, packet->data, 31) == -1) {
		fprintf(stderr, "failed to read /dev/random\n"); // @todo handle error ?
		close(fd);
		return ;
	}
	packet->data[31] = 0;
	close(fd);
}

/**
 * @see https://datatracker.ietf.org/doc/html/rfc792
 * @see netinet/ip_icmp.h
 * 
 * @brief   this is what an icmp echo/reply packet look like
 * 
 *          bits  0            8           16           24           32
 *                +--------------------------------------------------+
 *                +    type    |    code    |        checksum        +
 *                +--------------------------------------------------+
 *                +        identifier       |     sequence number    +
 *                +--------------------------------------------------+
 *                +                   optional data                  +
 *                +--------------------------------------------------+
 *
 */
icmp_echo_t *build_echo_packet(prog_t *prog) {
	static icmp_echo_t		icmp_echo_packet = {};

	set_type(&icmp_echo_packet, ICMP_ECHO);
	set_code(&icmp_echo_packet, ECHO_REQEST);
	set_identifier(&icmp_echo_packet, 0);
	set_sequence_number(&icmp_echo_packet);
	set_data(&icmp_echo_packet);

	icmp_echo_packet.checksum = checksum((void *)&icmp_echo_packet, sizeof(icmp_echo_t));

	return (&icmp_echo_packet);
}

/**
 * @brief   build an icmp packet depending on argument given to program (prog->opts)
 *          build an icmp echo by default if no program arguments says opposites
 *
 * @param[in, out] prog    [in]: indicates program options     
 *                         [out]: attach icmp packet built to prog->icmp_packet
 */
void build_icmp_packet(prog_t *prog) {

	if (prog->opts.disabled_echo == false)
		prog->icmp_packet = (void *)build_echo_packet(prog);

}

int init_networking(prog_t *prog) {
	if (retreive_host_infos(prog) == NO_HOST_GIVEN)
		return (NETWORKING_ERROR);

	build_icmp_packet(prog);

	return (NETWORKING_SUCCESS);
}

int send_packet(prog_t *prog) {
	return (NETWORKING_SUCCESS);
}