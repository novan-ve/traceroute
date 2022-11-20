#include "traceroute.h"
#include <unistd.h>

void print_help() {
    dprintf(2, "Usage:\n");
    dprintf(2, "  traceroute [ -h ] host\n");
    dprintf(2, "Options:\n");
    dprintf(2, "  -h --help    Read this help and exit\n");
    dprintf(2, "\nArguments:\n");
    dprintf(2, "+      host    The host to traceroute to\n");
}

int args_parser(int ac, char **av, int *host_i, int *len_i) {
    for (int i = 1; i < ac; i++) {
        if (av[i][0] == '-') {
            if (!ft_strcmp("-h", av[i]) || !ft_strcmp("--help", av[i])) {
                print_help();
                return (0);
            }
            else if (av[i][1]) {
                dprintf(2, "Bad option `-%c' (argc %d)\n", av[i][1], i);
                return (2);
            }
        }
        if (*host_i) {
            if (*len_i) {
                dprintf(2, "Extra arg `%s' (position %d, argc %d)\n", av[i], i, i);
                return (2);
            }
            *len_i = i;
        }
        else {
            *host_i = i;
        }
    }
    if (*len_i && !is_number(av[*len_i])) {
        dprintf(2, "Cannot handle \"packetlen\" cmdline arg `%s'", av[*len_i]);
        dprintf(2, " on position %d (argc %d)\n", *len_i, *len_i);
        return (2);
    }
    if (!*host_i) {
        print_help();
        return (0);
    }
    return (1);
}

int main(int ac, char **av) {
    char ip[64];
    int host_i = 0;
    int len_i = 0;
    int pkt_len = 0;
    int res;

    res = args_parser(ac, av, &host_i, &len_i);

    if (res != 1) {
        return (res);
    }
    if (getuid()) {
        dprintf(2, "traceroute: usage error: command requires root privileges\n");
        return (2);
    }
	if (resolve_dns(ip, av[host_i], 64)) {
        dprintf(2, "%s: Name or service not known\n", av[host_i]);
        dprintf(2, "Cannot handle \"host\" cmdline arg `%s' on position %d (argc %d)\n",
                av[host_i], host_i, host_i);
        return (2);
    }
    if (!len_i) {
        pkt_len = DEFAULT_PKG_LEN;
    }
    else {
        pkt_len = ft_atoi(av[len_i]);
        if (pkt_len < 28) {
            pkt_len = 28;
        }
        else if (pkt_len > 65000) {
            dprintf(2, "too big packetlen %d specified\n", pkt_len);
            return (2);
        }
    }
    printf("traceroute to %s (%s), %d hops max, %d byte packets\n",
           av[host_i], ip, MAX_TTL, pkt_len);

    exit(trace_loop(ip, pkt_len));
}
