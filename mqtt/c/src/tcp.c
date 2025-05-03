#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "tcp.h"

int connect_tcp(const char *host, int port) {
    int sock;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("ERR: ソケットの作成に失敗しました");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, host, &server.sin_addr);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("ERR: サーバーへの接続に失敗しました");
        return -1;
    }

    return sock;
}

int disconnect_tcp(int sock) {
    if (close(sock) < 0) {
        perror("ERR: ソケットのクローズに失敗しました");
        return -1;
    }
    return 0;
}
