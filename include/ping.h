#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#pragma pack(1)

typedef struct icmp_echo_s {
	uint8_t		type;
	uint8_t		code;
	uint16_t	checksum;
	uint16_t	identifier;
	uint16_t	sequence_number;
	char		data[32];
}	icmp_echo_t;

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
	bool	disabled_echo;
}	opt_t;

/**
 * @brief main program structure
 *
 *   contains:
 * - program options
 * - list of given IP/hostnames
 */
typedef struct prog_s {
	opt_t		opts;
	ip_t		*ip_list;
	//icmp_echo_t	icmp_echo_pckt;
	void		*icmp_packet;
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