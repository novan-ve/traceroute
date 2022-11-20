#include "traceroute.h"

#include <sys/socket.h>
#include <netdb.h>

void print_code(int code, int *unreachable, int *done) {
    if (code) {
        (*unreachable)++;
    }
    if (code - 1 == ICMP_UNREACH_NET)
        printf(" !N");
    else if (code - 1 == ICMP_UNREACH_HOST)
        printf(" !H");
    else if (code - 1 == ICMP_UNREACH_PROTOCOL)
        printf(" !P");
    else if (code - 1 == ICMP_UNREACH_NEEDFRAG)
        printf(" !F");
    else if (code - 1 == ICMP_UNREACH_SRCFAIL)
        printf(" !S");
    else if (code - 1 == ICMP_UNREACH_HOST_PROHIB)
        printf(" !X");
    else if (code - 1 == ICMP_UNREACH_HOST_PRECEDENCE)
        printf(" !V");
    else if (code - 1 == ICMP_UNREACH_PRECEDENCE_CUTOFF)
        printf(" !C");
    else if (code - 1 == ICMP_UNREACH_PORT)
        (*done)++;
    else if (code)
        printf(" !%d", code - 1);
}

void print_host(struct sockaddr_in *addr, double rtt) {
    static uint32_t last_addr = 0;
    char host[NI_MAXHOST];
    char *ip;

    if (last_addr != addr->sin_addr.s_addr || rtt <= 0) {
        ip = inet_ntoa(addr->sin_addr);
        ft_memset(host, 0, NI_MAXHOST);
        if (!getnameinfo((struct sockaddr*)addr, sizeof(*addr),
                         host, sizeof(host), NULL, 0, 0)) {
            printf(" %s (%s)", host, ip);
            last_addr = addr->sin_addr.s_addr;
            return;
        }
        if (!ft_strcmp(ip, "127.0.0.1"))
            printf("localhost (%s)", ip);
        else
            printf(" %s (%s)", ip, ip);
        last_addr = addr->sin_addr.s_addr;
    }
}

int trace_loop(char *ip, int packet_len) {
    int send_socket;
    int recv_socket;
    char recv_buf[BUFFER_SIZE];
    struct sockaddr_in recv_addr;
    int unreachable = 0;
    int done = 0;
    int code = 0;

    if ((send_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        dprintf(2, "socket: failed to create UDP socket\n");
        return (2);
    }
    if ((recv_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
        dprintf(2, "socket: failed to create raw socket\n");
        return (2);
    }

    for (int ttl = 1; ttl <= MAX_TTL; ttl++) {
        double rtt = 0.0;
        printf("%2d  ", ttl);
        for (int probe = 1; probe <= NB_PROBES; probe++) {
            double send_time = time_in_ms();
            send_packet(send_socket, ip, packet_len, ttl, probe);
            while (1) {
                int bytes_received = recv_packet(recv_socket, &recv_addr, recv_buf, rtt, ttl);
                if (bytes_received > 0) {
                    code = unpack_packet(recv_buf, bytes_received);
                    if (code == -1) {
                        continue;
                    }
                    print_host(&recv_addr, rtt);
                    rtt = time_in_ms() - send_time;
                    printf("  %.3f ms", rtt);
                    print_code(code, &unreachable, &done);
                }
                else {
                    printf(" *");
                    rtt = -1;
                }
                break;
            }
        }
        printf("\n");
        if (done || unreachable >= NB_PROBES - 1) {
            exit(0);
        }
    }

    return (0);
}
