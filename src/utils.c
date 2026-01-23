#include "utils.h"
#include "messages.h"
#include "ping.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

int ft_strlen(const char *string) {
	if (!string)
		return (0);

	int i = 0;
	while (string[i]) i++;

	return (i);
}

int print_err(const char *error, int return_value) {
	fprintf(stderr, "%s\n", error);
	return (return_value);
}

int print_help() {
	return (0);
}

int print_hint() {
	printf("Try 'ft_ping --help' or 'ft_ping --usage' for more information.\n");
	return (2);
}

void zero_memory(void *ptr, size_t size) {
	char *lptr = (char *)ptr;

	for (size_t i = 0; i < size; i++) {
		lptr[i] = 0;
	}
}

static void tabs(int nb) {
	for (int i = 0; i < nb; i++) {
		printf("\t");
	}
}

static void print_ip_list(ip_t *ip_list) {
	// == ip list
	tabs(1);
	printf("ip_t ip_list {\n");

	int count = 0;
	while (ip_list) {
		tabs(2);
		printf("node = %i\n", count);
		tabs(2);
		printf("------------------------------------------------\n");
		tabs(2);
		printf("char *raw_hostname = %s\n", ip_list->raw_hostname);
		tabs(2);
		printf("uint decimal_ip    = %u\n", ip_list->decimal_ip);
		//tabs(2);
		ip_list = ip_list->next;
		count++;
	}
	// == closing ip list
	tabs(1);
	printf("}\n\n");
}

static void print_opts(opt_t opts) {
	// == opts ==
	tabs(1);
	printf("opt_t opts {\n");
	tabs(2);
	printf("int host_count = %d;\n", opts.host_count);
	tabs(2);
	printf("char verbose   = %i;\n", opts.verbose);
	tabs(2);
	printf("int end_opt    = %i;\n", opts.end_opt);
	tabs(1);
	// == closing opts
	printf("}\n\n");
}

static void print_icmp_packet(void *icmp_packet) {
	// == echo packet
	tabs(1);
	printf("icmp_echo_t icmp_echo_pckt {\n");
	tabs(2);
	printf("uint8_t type             = %u;\n", ((icmp_echo_t *)icmp_packet)->type);
	tabs(2);
	printf("uint8_t code             = %u;\n", ((icmp_echo_t *)icmp_packet)->code);
	tabs(2);
	printf("uint16_t checksum        = 0x%X;\n", ((icmp_echo_t *)icmp_packet)->checksum);
	tabs(2);
	printf("uint16_t identifier      = %u;\n", ((icmp_echo_t *)icmp_packet)->identifier);
	tabs(2);
	printf("uint16_t sequence_number = %u;\n", ((icmp_echo_t *)icmp_packet)->sequence_number);
	//tabs(2);
	//printf("char data[32]            = %s;\n", ((icmp_echo_t *)icmp_packet)->data);
	// == closing echo packet
	tabs(1);
	printf("}\n\n");
}

void print_struct(prog_t *prog) {
	// == prog ==
	printf("prog_t prog {\n\n");

	print_opts(prog->opts);

	print_ip_list(prog->ip_list);

	print_icmp_packet((void *)prog->icmp_packet);

	// == closing prog
	printf("}\n");
}

void push_back_new_host(ip_t **head, char *hostname) {
	ip_t *new_node = malloc(sizeof(ip_t));

	zero_memory((void *)new_node, sizeof(ip_t));
	new_node->raw_hostname = hostname;

	if (!(*head)) {
		*head = new_node;
		return ;
	}

	ip_t *actual = *head;
	while (actual && actual->next) {
		actual = actual->next;
	}

	actual->next = new_node;
}

size_t host_list_size(ip_t *head) {
	size_t i = 0;

	while (head) {
		head = head->next;
		i++;
	}
	return (i);
}

void clean_packet(icmp_echo_t *packet) {
	//if (packet->data) free(packet->data);
	if (packet) free(packet);
}

//! @param[in] decimal_addr	give it in host byte order
const char *ascii_addr_from_decimal(int decimal_addr, char *buffer) {
	struct in_addr addr = {};

	addr.s_addr = htonl(decimal_addr);
	return (inet_ntop(AF_INET, &addr, buffer, INET_ADDRSTRLEN));
}


void print_verbose_unreachable(opt_t *options, icmp_error_packet_t *packet) {
	if (!options->verbose) return ;

	uint16_t *ip_hdr = (uint16_t *)&packet->original_ip;
	printf("IP Hdr Dump: \n");
	for (int i = 0; i < 10; i++) {
		printf(" %04x", ntohs(ip_hdr[i]));
	}

	char		src_buf[INET_ADDRSTRLEN] = {},\
				dst_buf[INET_ADDRSTRLEN] = {};

	ascii_addr_from_decimal(ntohl(packet->original_ip.saddr), src_buf);
	ascii_addr_from_decimal(ntohl(packet->original_ip.daddr), dst_buf);

	struct iphdr *ip_header = (struct iphdr *)&packet->original_ip;
	printf("\n");
	printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data\n");
	printf(" %x  %x  %02x %04x %04x   %x %04x  %x  %02x %x %s  %s\n",
			ip_header->version,
			ip_header->ihl,
			ip_header->tos,
			ntohs(ip_header->tot_len),
			ntohs(ip_header->id),
			ntohs(ip_header->frag_off) >> 13,
			ntohs(ip_header->frag_off) & 0x1FFF,
			ip_header->ttl,
			ip_header->protocol,
			ntohs(ip_header->check),
			src_buf,
			dst_buf
		);
}

void print_header(ip_t *client, opt_t opts, uint16_t identifier) {
	char ip[INET_ADDRSTRLEN] = {};

	ascii_addr_from_decimal(client->decimal_ip, ip);

	printf("FT_PING %s (%s): %d data bytes", client->raw_hostname, ip, opts.data_size);
	if (opts.verbose)
		printf(", id 0x%x = %u", identifier, identifier);
	printf("\n");
}

void print_stats(void) {

}