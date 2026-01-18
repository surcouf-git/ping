#pragma once

#include "ping.h"

int init_networking(prog_t *prog);
int send_packet(prog_t *prog);
uint16_t checksum(const void *data, size_t length);