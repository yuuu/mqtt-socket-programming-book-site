# frozen_string_literal: true

require 'socket'

SERVER_HOST = '127.0.0.1'
SERVER_PORT = 49_152

socket = Socket.new(Socket::AF_INET, Socket::SOCK_STREAM)
sockaddr = Socket.sockaddr_in(SERVER_PORT, SERVER_HOST)
socket.connect(sockaddr)

puts 'Enter messages to send to the server.'

begin
  loop do
    print '> '
    message = gets.strip
    next if message.empty?

    socket.send(message, 0)
    response = socket.recv(1024)
    puts "Received: #{response.strip}"
  end
rescue Interrupt
  puts "\nClient shutting down..."
ensure
  socket.close
end
