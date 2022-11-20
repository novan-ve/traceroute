#include "traceroute.h"

#include <netdb.h>

int	resolve_dns(char *dst, char *src, size_t dst_size) {
    struct addrinfo		hints;
    struct addrinfo		*servinfo;
    struct addrinfo		*result;
    struct sockaddr_in	*sockinfo;
    char                *addr;

    ft_memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(src, NULL, &hints, &servinfo)) {
        return (1);
    }

    result = servinfo;

    while (result) {
        sockinfo = (struct sockaddr_in *)result->ai_addr;

        addr = inet_ntoa(sockinfo->sin_addr);
        if (addr) {
            ft_memcpy(dst, addr, dst_size);
            return (0);
        }

        result = result->ai_next;
    }
    return (1);
}

int validate_checksum(void *b, size_t len) {
    struct icmp *packet = (struct icmp*)b;
    unsigned short cksum = packet->icmp_cksum;
    unsigned short *addr = (unsigned short*)b;
    unsigned short result;
    long sum = 0;

    packet->icmp_cksum = 0;

    while (len > 1) {
        sum += *addr++;
        len -= 2;
    }

    if (len > 0) {
        sum += *(unsigned char*)addr;
    }

    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    result = ~sum;
    packet->icmp_cksum = cksum;

    return (result == cksum);
}