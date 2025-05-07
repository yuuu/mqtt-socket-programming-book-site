# frozen_string_literal: true

require 'socket'

def send_mqtt_connect_packet(sock, client_id)
  packet = []

  # Fixed Header
  packet << 0x10 # MQTT Control Packet type (1) + Reserved (0)

  # Remaining Length
  variable_header_len = 10
  payload_len = 2 + client_id.bytesize
  packet << (variable_header_len + payload_len)

  # Variable Header
  packet += [0x00, 0x04, 'M'.ord, 'Q'.ord, 'T'.ord, 'T'.ord, 0x04, 0x02, 0x00, 0x3C]

  # Payload
  packet += [0x00, client_id.bytesize]
  packet += client_id.bytes

  sock.send(packet.pack('C*'), 0)
end

def receive_mqtt_connack_packet(sock)
  packet = sock.recv(4).unpack('C*')
  return false if packet != [0x20, 0x02, 0x00, 0x00]

  puts 'MQTT 接続成功'
  true
end

def send_mqtt_publish_packet(sock, topic, message)
  # Fixed Header
  packet = [0x30] # MQTT Control Packet type (3) + Reserved (0)

  # Remaining Length
  topic_len = topic.bytesize
  variable_header_len = 2 + topic_len
  payload_len = message.bytesize
  packet << (variable_header_len + payload_len)

  # Variable Header
  packet += [0x00, topic_len]
  packet += topic.bytes

  # Payload
  packet += message.bytes

  sock.send(packet.pack('C*'), 0)
end

def send_mqtt_subscribe_packet(sock, topic)
  # Fixed Header
  packet = [0x82] # MQTT Control Packet type (8) + Reserved (0)

  # Remaining Length
  topic_len = topic.bytesize
  variable_header_len = 2
  payload_len = 2 + topic_len + 1
  packet << (variable_header_len + payload_len)

  # Variable Header
  packet += [0x00, 0x01]

  # Payload
  packet += [0x00, topic_len]
  packet += topic.bytes
  packet << 0x00

  sock.send(packet.pack('C*'), 0)
end

def receive_mqtt_suback_packet(sock)
  packet = sock.recv(5).unpack('C*')
  return false if packet != [0x90, 0x03, 0x00, 0x01, 0x00]

  puts 'MQTT 購読成功'
  true
end

def receive_mqtt_publish_packet(sock)
  packet = sock.recv(32).unpack('C*')
  return false if packet[0] != 0x30

  topic_length = (packet[2] << 8) | packet[3]
  topic = packet[4, topic_length].pack('C*')
  message = packet[(4 + topic_length)..(1 + packet[1])].pack('C*')

  puts 'MQTT PUBLISH パケットを受信しました'
  puts "トピック: #{topic}, メッセージ: #{message}"
  true
end

def send_mqtt_disconnect_packet(sock)
  packet = [0xE0, 0x00]
  sock.send(packet.pack('C*'), 0)
end
