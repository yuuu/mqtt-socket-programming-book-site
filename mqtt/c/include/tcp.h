#ifndef TCP_H
#define TCP_H

int connect_tcp(const char *host, int port);
int disconnect_tcp(int sock);

#endif // TCP_H
