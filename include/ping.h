#pragma once

#include <stdint.h>
#include <stddef.h>

#pragma pack(1)

typedef struct icmp_hdr_s {
	uint8_t		type;
	uint8_t		code;
	uint16_t	checksum;
}	icmp_hdr_t;

typedef struct icmp_s {
	icmp_hdr_t	header;
	void		*body;
}	icmp_t;

#pragma pack(0)

typedef struct ip_s {
	char			*raw_hostname;
	unsigned int	decimal_ip;
	struct ip_s		*next;
}	ip_t;

typedef struct opt_s {
	char	verbose;
	int		end_opt;
	int		host_count;
}	opt_t;

/**
 * @brief main program structure
 *
 *   contains:
 * - program options
 * - list of given IP/hostnames
 * - raw ethernet frame
 */
typedef struct prog_s {
	opt_t	opts;
	ip_t	*ip_list;
	icmp_t	icmp_pckt;
}	prog_t;

#define PARSE_ERROR 0
#define PARSE_OK 1

#define NETWORKING_ERROR 0
#define NETWORKING_SUCCESS 1

#define VALID_HOST 1
#define NO_HOST_GIVEN 0

#define ECHO_REQEST 0

// init
void	init(prog_t *prog);