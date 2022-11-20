#ifndef TRACEROUTE_H
#define TRACEROUTE_H

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

#define MAX_TTL 30
#define DEFAULT_PKG_LEN 60
#define NB_PROBES 3
#define BUFFER_SIZE 512

int trace_loop(char *ip, int packet_len);

void send_packet(int send_socket, char *ip, int packet_len, int ttl, int probe);
int recv_packet(int socket, struct sockaddr_in *addr,
                char *buf, double rtt, int ttl);
int unpack_packet(char *packet_str, int size);

int	resolve_dns(char *dst, char *src, size_t dst_size);
int validate_checksum(void *b, size_t len);

void *ft_memset(void *b, int c, size_t len);
void *ft_memcpy(void *dst, const void *src, size_t n);
int ft_strcmp(const char *s1, const char *s2);
int ft_atoi(const char *str);
int is_number(const char *s);
double time_in_ms();

#endif