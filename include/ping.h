/**
 * @file ping.h
 * 
 * @author surcouf-git
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>

#pragma pack(1)

typedef struct icmp_echo_s {
	uint8_t		type;
	uint8_t		code;
	uint16_t	checksum;
	uint16_t	identifier;
	uint16_t	sequence_number;
}	icmp_echo_t;

typedef struct icmp_error_packet_s {
	struct iphdr	ip;
	struct icmphdr	icmp;
	struct iphdr	original_ip;
	icmp_echo_t		original_icmp;
}	icmp_error_packet_t;

typedef struct icmp_echo_reply_s {
	char			garbage[8];
	uint8_t			ttl;
	uint8_t			proto;
	uint16_t		hdr_checksum;
	unsigned int	ip_source;
	unsigned int	ip_dest;
	icmp_echo_t		icmp_packet;
}	icmp_echo_reply_t;

#pragma pack(0)

typedef struct ip_s {
	char			*raw_hostname;
	char			*hostname;
	unsigned int	decimal_ip;
	struct ip_s		*next;
}	ip_t;

typedef struct opt_s {
	long		ping_count;
	char		verbose;
	int			end_opt;
	int			host_count;
	bool		disabled_echo;
	int			data_size;
}	opt_t;

typedef struct elapsed_time_s {
	struct timespec	start_time;
	struct timespec	end_time;
}	elapsed_time_t;

/**
 * @struct prog_s main program structure
 *   contains:
 * - program options
 * - list of given IP/hostnames
 * - icmp packet
 */
typedef struct prog_s {
	int			socket;
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
#define ICMP_ECHO 8

#define PTR_SIZE 8
#define ICMP_STRUCT_SIZE (sizeof(icmp_echo_t) - PTR_SIZE)

#define IP_HDR_SIZE 20

#define REPLY_PACKET_SIZE ((sizeof(icmp_echo_reply_t) + data_size))

#define RECV_CONTINUE 1

#define MAX_DATA_SIZE 1024
#define MAX_PACKET_SIZE (MAX_DATA_SIZE + sizeof(icmp_error_packet_t))