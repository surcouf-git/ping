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
		gai_strerror(errno);
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

static void set_type(icmp_hdr_t *packet, int type) {
	packet->type = type;
}

static void set_code(icmp_hdr_t *packet, int code) {
	packet->code = code;
}

static void checksum(icmp_hdr_t *packet) {
	
}

/**
 * Doxygen here (netinet/ip_icmp.h)
 */
static void build_icmp_packet(prog_t *prog) {
	icmp_hdr_t	icmp_packet = {};

	set_type(&icmp_packet, ICMP_ECHO);
	set_code(&icmp_packet, ECHO_REQEST);
	checksum(&icmp_packet);
}

int init_networking(prog_t *prog) {
	if (retreive_host_infos(prog) == NO_HOST_GIVEN)
		return (NETWORKING_ERROR);

	build_icmp_packet(prog);
	
	return (NETWORKING_SUCCESS);
}