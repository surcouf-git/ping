#pragma once

#include "ping.h"

#define ENABLE_OPT 1
#define PARSE_STOP -1

int parse_input(int argc, char **argv, prog_t *prog);
int check_argument_nb(int argc);