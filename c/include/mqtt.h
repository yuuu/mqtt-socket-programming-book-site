#ifndef MQTT_H
#define MQTT_H

#define MQTT_PORT 1883
#define MQTT_SERVER "127.0.0.1"
#define MQTT_TOPIC "test_topic"
#define MQTT_MESSAGE "hello"

int send_mqtt_connect_packet(int sock, char* client_id);
int receive_mqtt_connack_packet(int sock);
int send_mqtt_publish_packet(int sock);
int send_mqtt_disconnect_packet(int sock);
int send_mqtt_subscribe_packet(int sock);
int receive_mqtt_suback_packet(int sock);
int receive_mqtt_publish_packet(int sock);

#endif // MQTT_H
