#include "traceroute.h"

#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>

void send_packet(int send_socket, char *ip, int packet_len, int ttl, int probe) {
    struct sockaddr_in addr;
    int body_size = packet_len - 28;
    char packet[body_size];
    int ret;

    if (setsockopt(send_socket, IPPROTO_IP, IP_TTL, (char *)&ttl, sizeof(ttl)) == -1) {
        dprintf(2, "traceroute: setsockopt: failed to set ttl for UDP socket\n");
        return;
    }

    ft_memset(&addr, 0, sizeof(addr));
    ft_memset(packet, 0, body_size);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(33434 + ttl + probe - 1);
    if (!inet_pton(AF_INET, ip, &(addr.sin_addr))) {
        dprintf(2, "inet_pton: invalid address\n");
        return;
    }

    ret = sendto(send_socket, packet, body_size, 0, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1 || ret != body_size) {
        dprintf(2, "sendto: failed\n");
        dprintf(2, "traceroute: wrote %s %d chars, ret=%d\n", ip, body_size, ret);
    }
}

int recv_packet(int socket, struct sockaddr_in *addr, char *buf, double rtt, int ttl) {
    struct timeval tv;
    unsigned int addrlen;
    int ret = 0;
    fd_set fds;

    tv.tv_sec = 0;
    if (ttl == 1 && rtt <= 0) {
        tv.tv_usec = 10000;
    }
    else {
        tv.tv_usec = 1000000 / 3;
    }

    FD_ZERO(&fds);
    FD_SET(socket, &fds);

    ft_memset(buf, 0, BUFFER_SIZE);
    ft_memset(addr, 0, sizeof(*addr));

    addrlen = sizeof(*addr);

    if (select(socket + 1, &fds, NULL, NULL, &tv) > 0) {
        ret = recvfrom(socket, buf, BUFFER_SIZE, 0,
                       (struct sockaddr*)addr, &addrlen);
    }
    return (ret);
}

int unpack_packet(char *packet_str, int size) {
    struct ip *ip_packet;
    struct icmp *icmp_packet;
    int ip_hdrlen;

    ip_packet = (struct ip*)packet_str;
    ip_hdrlen = ip_packet->ip_hl << 2;
    icmp_packet = (struct icmp*)(packet_str + ip_hdrlen);

    if (ip_hdrlen + ICMP_MINLEN > size || 
        !validate_checksum(icmp_packet, size) ||
        !ip_packet->ip_ttl) {
        return (-1);
    }
    if (icmp_packet->icmp_type == ICMP_TIMXCEED) {
        return (0);
    }
    if (icmp_packet->icmp_type == ICMP_UNREACH) {
        return icmp_packet->icmp_code + 1;
    }
    return (-1);
}