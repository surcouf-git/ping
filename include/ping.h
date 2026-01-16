#pragma once

#include <stdint.h>
#include <stddef.h>

#pragma pack(1)

typedef struct frame_s {
	uint8_t		destination[6];
	uint8_t		source[6];
	uint16_t	type;
	// ICMP packet
}	frame_t;

#pragma pack(0)

typedef struct opt_s {
	char	verbose;
	int		end_opt;
}	opt_t;

typedef struct ip_s {
	char		*raw_hostname;
	char		*formated_hostname;
	char		*ascii_ip;
	long		decimal_ip;
	struct ip_s	*next;
}	ip_t;

/* 
 * @brief main program structure
 *
 *   contains:
 * - program options
 * - list of given IP/hostnames
 * - raw ethernet frame
 */
typedef struct prog_s {
	opt_t	opts;
	frame_t	frame;
	ip_t	*ip_list;
}	prog_t;

#define PARSE_ERROR 0
#define PARSE_OK 1

#define NETWORKING_ERROR 0
#define NETWORKING_SUCCESS 1

// init
void	init(prog_t *prog);