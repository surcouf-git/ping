#include "networking.h"
#include "ping.h"
#include "utils.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>

/**
 * @todo working on retreiving addr or hostname info
 */

static void retreive_host_infos(prog_t *prog) {
	size_t host_nb = host_list_size(prog->ip_list), i = 0;

	
	struct addrinfo hints = {
						.ai_family = AF_INET,
						.ai_socktype = 0
					}, 
					*response = NULL;

	printf ("ai_flags:%i hnb:%ld\n", hints.ai_flags, host_nb);

	ip_t *ptr = prog->ip_list;
	while (i < host_nb) {
		errno = 0;
		if (getaddrinfo(ptr->raw_hostname, NULL, &hints, &response) == 0) {
			struct sockaddr_in *addr = (struct sockaddr_in *)response->ai_addr;
			printf("type: %d name: %s", addr->sin_family, response->ai_addr->sa_data);
			//for (int i = 0; addr->sa_data[i]; i++) {
			//	printf("%c ", response->ai_addr->sa_data[i]);
			//}
			//printf("%d\n", addr->sin_addr.s_addr);
			printf("\n");
		} else {
			int err = errno;
			printf("failed to retreive %s\n", ptr->raw_hostname);
			gai_strerror(err);
		}
		i++;
		ptr = ptr->next;
	}
}

int init_networking(prog_t *prog) {
	retreive_host_infos(prog);
	return (NETWORKING_SUCCESS);
}