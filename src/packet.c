/**
 * @file packet.c
 * 
 * @author surcouf-git
 */

#include "ping.h"
#include "networking.h"
#include "utils.h"
#include "messages.h"

#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <stdio.h>

static void set_type(icmp_echo_t *packet, uint8_t type) {
	packet->type = type;
}

static void set_code(icmp_echo_t *packet, uint8_t code) {
	packet->code = code;
}

static void set_identifier(icmp_echo_t *packet, uint16_t identifier) {
	packet->identifier = identifier;
}

static void set_sequence_number(icmp_echo_t *packet, uint16_t sequence_number) {
	packet->sequence_number = sequence_number;
}

uint16_t checksum(const void *data, size_t length) {
	const uint16_t *ptr = data;
	uint32_t sum = 0;

	while (length > 1) {
		sum += *ptr++;
		length -= 2;
	}

	if (length == 1)
		sum += *((const uint8_t *)ptr);

	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	return (uint16_t)(~sum);
}

/**
 * @todo doxygen ?
 */
static void set_data(icmp_echo_t *packet) {

	int fd = open("/dev/random", O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "cannot open /dev/random\n"); //! @todo handle error ?
		return ;
	}

	if (read(fd, packet->data, 31) == -1) {
		fprintf(stderr, "failed to read /dev/random\n"); //! @todo handle error ?
		close(fd);
		return ;
	}
	packet->data[31] = 0;
	close(fd);
}

/**
 * @brief   build the icmp packet. this is what an icmp echo/reply packet look like
 *
 @verbatim
 bits  0            8           16           24           32
       +--------------------------------------------------+
       +    type    |    code    |        checksum        +
       +--------------------------------------------------+
       +        identifier       |     sequence number    +
       +--------------------------------------------------+
       +                   optional data                  +
       +--------------------------------------------------+
 @endverbatim
 * 
 * @return fullfilled icmp echo packet (struct icmp_echo_s)
 *
 * @see https://datatracker.ietf.org/doc/html/rfc792
 * @see netinet/ip_icmp.h
 */
icmp_echo_t build_echo_packet(uint16_t identifier, uint16_t sequence_number) {
	icmp_echo_t		icmp_echo_packet = {};

	set_type(&icmp_echo_packet, ICMP_ECHO);
	set_code(&icmp_echo_packet, ECHO_REQEST);
	set_identifier(&icmp_echo_packet, identifier);
	set_sequence_number(&icmp_echo_packet, sequence_number);
	set_data(&icmp_echo_packet);

	icmp_echo_packet.checksum = checksum((void *)&icmp_echo_packet, sizeof(icmp_echo_t));

	return (icmp_echo_packet);
}

/**
 * @brief   build an icmp packet depending on argument given to program (prog->opts)
 *          build an icmp echo by default if no program arguments says opposites
 *
 * @param[in, out] prog    [in]: indicates program options     
 *                         [out]: attach icmp packet built to prog->icmp_packet
 */
void build_icmp_packet(prog_t *prog) {

	//if (prog->opts.disabled_echo == false)
	//	prog->icmp_packet = (void *)build_echo_packet(prog);

}