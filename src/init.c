#include "utils.h"
#include "ping.h"

void init(prog_t *prog) {
	zero_memory((char *)prog, sizeof(prog_t));
}