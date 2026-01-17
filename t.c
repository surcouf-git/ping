#include <stdio.h>
#include <stdint.h>

//#pragma pack(1)

typedef struct icmp_hdr_s {
	uint8_t		type;
	uint8_t		code;
	uint16_t	checksum;
}	__attribute__((packed)) icmp_hdr_t;

typedef struct s {
	icmp_hdr_t	header;
	void		*body;
} __attribute__((packed)) t;

//#pragma pack()


int main () {
	printf("size: %u\n", sizeof(t));
}