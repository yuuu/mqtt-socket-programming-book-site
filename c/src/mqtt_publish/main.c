#include "tcp.h"
#include "mqtt.h"

#define MQTT_CLIENT_ID "publisher"

int main() {
    int sock;

    sock = connect_tcp(MQTT_SERVER, MQTT_PORT);
    if (sock < 0) {
        return -1;
    }
    if (send_mqtt_connect_packet(sock, MQTT_CLIENT_ID) < 0) {
        disconnect_tcp(sock);
        return -1;
    }
    if (receive_mqtt_connack_packet(sock) < 0) {
        disconnect_tcp(sock);
        return -1;
    }
    if (send_mqtt_publish_packet(sock) < 0) {
        disconnect_tcp(sock);
        return -1;
    }
    if (send_mqtt_disconnect_packet(sock) < 0) {
        disconnect_tcp(sock);
        return -1;
    }
    if (disconnect_tcp(sock) < 0) {
        return -1;
    }

    return 0;
}
