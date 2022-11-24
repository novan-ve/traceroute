# Traceroute

Reimplementation of the traceroute command in C.

```bash
$ make && ./ft_traceroute google.com
traceroute to google.com (142.251.33.4), 30 hops max, 60 byte packets
1  63.133.178.109 (63.133.178.109)  0.210 ms  0.268 ms  0.214 ms
2  * * *
3  ae1.cr0-dal4.ip4.gtt.net (69.174.3.1)  0.311 ms  0.317 ms  0.354 ms
4  ae1.cr10-dal3.ip4.gtt.net (213.254.230.210)  1.260 ms  2.320 ms  2.304 ms
5  as15169.dal33.ip4.gtt.net (199.229.230.118)  3.595 ms  2.464 ms  1.365 ms
6  74.162.39.232 (74.162.39.232)  4.859 ms  1.875 ms  1.955 ms
7. * * *
8  108.170.240.129 (108.170.240.129)  2.636 ms 108.170.240.193 (108.170.240.193)  1.501 ms 108.170.240.129 (108.170.240.129)  2.620 ms
9  142.251.60.143 (142.251.60.143)  1.480 ms  1.499 ms 142.251.60.145 (142.251.60.145)  1.473 ms
10 dfw25s44-in-f4.1e100.net (142.251.33.4)  1.425 ms  1.622 ms  1.587 ms
```
