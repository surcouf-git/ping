#include "utils.h"
#include "ping.h"

void init(prog_t *prog) {
	zero_memory((void *)prog, sizeof(prog_t));
}