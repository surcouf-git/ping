#pragma once

#include <stdint.h>
#include <stddef.h>

#define ENABLED 1
#define UNKNOWN -1

#pragma pack(1)

typedef struct frame_s {
	uint8_t		destination[6];
	uint8_t		source[6];
	uint16_t	type;
	// ICMP packet
}	frame_t;

#pragma pack(0)

typedef struct opt_s {
	char	help;
	char	verbose;
}	opt_t;

typedef struct prog_s {
	opt_t	opts;
	frame_t	frame;
}	prog_t;

// init
void	init(prog_t *prog);