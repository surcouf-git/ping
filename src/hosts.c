#include "ping.h"
#include "networking.h"
#include "utils.h"
#include "messages.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

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
		host_ptr->decimal_ip = ntohl(addr->sin_addr.s_addr);
	} else {
		gai_strerror(errno); // TODO return what on error ?
	}
	freeaddrinfo(response);
	return (VALID_HOST);
}

int retreive_host_infos(prog_t *prog) {
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