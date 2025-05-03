# frozen_string_literal: true

require_relative '../tcp'
require_relative '../mqtt'

MQTT_SERVER = '127.0.0.1'
MQTT_PORT = 1883
MQTT_CLIENT_ID = 'subscriber'
MQTT_TOPIC = 'test_topic'

begin
  sock = connect_tcp(MQTT_SERVER, MQTT_PORT)
  exit(-1) if sock.nil?

  unless send_mqtt_connect_packet(sock, MQTT_CLIENT_ID)
    disconnect_tcp(sock)
    exit(-1)
  end

  unless receive_mqtt_connack_packet(sock)
    disconnect_tcp(sock)
    exit(-1)
  end

  unless send_mqtt_subscribe_packet(sock, MQTT_TOPIC)
    disconnect_tcp(sock)
    exit(-1)
  end

  unless receive_mqtt_suback_packet(sock)
    disconnect_tcp(sock)
    exit(-1)
  end

  unless receive_mqtt_publish_packet(sock)
    disconnect_tcp(sock)
    exit(-1)
  end

  unless send_mqtt_disconnect_packet(sock)
    disconnect_tcp(sock)
    exit(-1)
  end

  disconnect_tcp(sock)
rescue StandardError => e
  puts "Error: #{e.full_message}"
  exit(-1)
end
