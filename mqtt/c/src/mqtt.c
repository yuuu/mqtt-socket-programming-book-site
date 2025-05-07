#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include "mqtt.h"

#define MQTT_FIXED_HEADER_SIZE 2

int send_mqtt_connect_packet(int sock, char* client_id) {
    char packet[32];
    int packet_len = 0;

    // Fixed Header
    packet[packet_len++] = 0x10; // MQTT Control Packet type (1) + Reserved (0)

    // Remaining Length
    unsigned char variable_header_len = 10;
    unsigned char payload_len = MQTT_FIXED_HEADER_SIZE + strlen(client_id);

    unsigned char remaining_len = variable_header_len + payload_len;
    packet[packet_len++] = remaining_len;

    // Variable Header
    packet[packet_len++] = 0x00; // Length MSB
    packet[packet_len++] = 0x04; // Length LSB
    packet[packet_len++] = 'M';  // Protocol Name
    packet[packet_len++] = 'Q';
    packet[packet_len++] = 'T';
    packet[packet_len++] = 'T';
    packet[packet_len++] = 0x04; // Protocol Level
    packet[packet_len++] = 0x02; // Connect Flags(Clean Session)
    packet[packet_len++] = 0x00; // Keep Alive MSB
    packet[packet_len++] = 0x3C; // Keep Alive LSB (60 seconds)

    // Payload
    packet[packet_len++] = 0x00;                            // Client ID Length MSB
    unsigned char client_id_len = strlen(client_id);        // Client ID Length LSB

    packet[packet_len++] = client_id_len;
    strncpy(&packet[packet_len], client_id, client_id_len); // Client ID
    packet_len += client_id_len;

    if (send(sock, packet, packet_len, 0) < 0) {
        perror("ERR: MQTT CONNECT パケットの送信に失敗しました");
        return -1;
    }
    return 0;
}

int receive_mqtt_connack_packet(int sock) {
    char packet[4];
    int packet_len;

    packet_len = recv(sock, packet, sizeof(packet), 0);
    if (packet_len < 0) {
        perror("ERR: MQTT CONNACK パケットの受信に失敗しました");
        return -1;
    }

    if (packet[0] != 0x20 || packet[1] != 0x02) {
        fprintf(stderr, "ERR: 不正な MQTT CONNACK パケット\n");
        return -1;
    }

    if (packet[2] != 0x00 || packet[3] != 0x00) {
        fprintf(stderr, "ERR: MQTT 接続失敗\n");
        return -1;
    }

    printf("MQTT 接続成功\n");
    return 0;
}

int send_mqtt_publish_packet(int sock) {
    char packet[32];
    int packet_len = 0;

    // Fixed Header
    packet[packet_len++] = 0x30; // MQTT Control Packet type (3) + Reserved (0)

    // Remaining Length
    unsigned char topic_len = strlen(MQTT_TOPIC); // Topic Length LSB
    unsigned char variable_header_len = MQTT_FIXED_HEADER_SIZE + topic_len;
    unsigned char payload_len = strlen(MQTT_MESSAGE); // Topic Length LSB

    unsigned char remaining_len = variable_header_len + payload_len;
    packet[packet_len++] = remaining_len;

    // Variable Header
    packet[packet_len++] = 0x00; // Topic Length MSB
    packet[packet_len++] = topic_len;
    strncpy(&packet[packet_len], MQTT_TOPIC, topic_len); // Topic
    packet_len += topic_len;

    // Payload
    strncpy(&packet[packet_len], MQTT_MESSAGE, payload_len); // Payload
    packet_len += payload_len;

    if (send(sock, packet, packet_len, 0) < 0) {
        perror("ERR: MQTT PUBLISH パケットの送信に失敗しました");
        return -1;
    }
    return 0;
}

int send_mqtt_subscribe_packet(int sock) {
    char packet[32];
    int packet_len = 0;

    // Fixed Header
    packet[packet_len++] = 0x82; // MQTT Control Packet type (8) + Reserved (0)

    // Remaining Length
    unsigned char variable_header_len = 2;
    unsigned char topic_len = strlen(MQTT_TOPIC);        // Topic Length LSB
    unsigned char payload_len = MQTT_FIXED_HEADER_SIZE + topic_len + 1;

    unsigned char remaining_len = variable_header_len + payload_len;
    packet[packet_len++] = remaining_len;

    // Variable Header
    packet[packet_len++] = 0x00; // Packet Identifier MSB
    packet[packet_len++] = 0x01; // Packet Identifier LSB

    // Payload
    packet[packet_len++] = 0x00;                         // Topic Length MSB
    packet[packet_len++] = topic_len;

    strncpy(&packet[packet_len], MQTT_TOPIC, topic_len); // Topic Name
    packet_len += topic_len;
    packet[packet_len++] = 0x00;

    if (send(sock, packet, packet_len, 0) < 0) {
        perror("ERR: MQTT SUBSCRIBE パケットの送信に失敗しました");
        return -1;
    }
    return 0;
}

int receive_mqtt_suback_packet(int sock) {
    unsigned char packet[5];
    int packet_len;

    packet_len = recv(sock, packet, sizeof(packet), 0);
    if (packet_len < 0) {
        perror("ERR: MQTT SUBACK パケットの受信に失敗しました");
        return -1;
    }

    if (packet[0] != 0x90 || packet[1] != 0x03 || packet[2] != 0x00 || packet[3] != 0x01) {
        fprintf(stderr, "ERR: 不正な MQTT SUBACK パケット\n");
        return -1;
    }

    if (packet[4] != 0x00) {
        fprintf(stderr, "ERR: MQTT 購読失敗\n");
        return -1;
    }

    printf("MQTT 購読成功\n");
    return 0;
}

int receive_mqtt_publish_packet(int sock) {
    char packet[32];
    int packet_len;

    packet_len = recv(sock, packet, sizeof(packet), 0);
    if (packet_len < 0) {
        perror("ERR: MQTT PUBLISH パケットの受信に失敗しました");
        return -1;
    }

    if (packet[0] != 0x30) {
        fprintf(stderr, "ERR: 不正な MQTT PUBLISH パケット\n");
        return -1;
    }

    unsigned char remaining_length = packet[1];
    unsigned short topic_length = (packet[2] << 8) | packet[3];
    char topic[topic_length + 1];
    strncpy(topic, &packet[4], topic_length);
    topic[topic_length] = '\0';

    unsigned short message_length = remaining_length - topic_length - MQTT_FIXED_HEADER_SIZE;
    char message[message_length + 1];
    strncpy(message, &packet[4 + topic_length], message_length);
    message[message_length] = '\0';

    printf("MQTT PUBLISH パケットを受信しました\n");
    printf("トピック: %s, メッセージ: %s\n", topic, message);

    return 0;
}

int send_mqtt_disconnect_packet(int sock) {
    char packet[4];
    int packet_len = 0;

    // Fixed Header
    packet[packet_len++] = 0xE0; // MQTT Control Packet type (14) + Reserved (0)

    // Remaining Length
    packet[packet_len++] = 0x00; // No payload

    if (send(sock, packet, packet_len, 0) < 0) {
        perror("ERR: MQTT DISCONNECT パケットの送信に失敗しました");
        return -1;
    }
    return 0;
}
