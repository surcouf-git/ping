#pragma once

#include "ping.h"

#include <sys/socket.h>
#include <netinet/in.h>

int init_networking(prog_t *prog);
uint16_t checksum(const void *data, size_t length);

//! @ref    src/networking.c
int routine(prog_t *prog);
#define ROUTINE_NEED_STOP 0
#define PROCEED_NEXT_CLIENT 1

//! @ref    src/hosts.c
int retreive_host_infos(prog_t *prog);

//! @ref    src/packet.c
icmp_echo_t *build_echo_packet(uint16_t identifier, uint16_t sequence_number, size_t data_size);

struct sockaddr_in fill_client_infos(ip_t client);

int receive_from_client(int socket, opt_t options, uint16_t identifier, size_t data_size, elapsed_time_t *timer);
int send_to_client(int socket, icmp_echo_t *packet, ip_t client, size_t data_size, elapsed_time_t *timer);

#define PACKET_COUNT (opts.ping_count ? packet_number < opts.ping_count : true)