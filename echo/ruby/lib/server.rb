# frozen_string_literal: true

require 'socket'

SERVER_HOST = '127.0.0.1'
SERVER_PORT = 49_152

socket = Socket.new(Socket::AF_INET, Socket::SOCK_STREAM)
sockaddr = Socket.sockaddr_in(SERVER_PORT, SERVER_HOST)
socket.bind(sockaddr)
socket.listen(1)

puts "Echo server is running on port #{SERVER_PORT}..."

begin
  loop do
    client, = socket.accept

    Thread.new(client) do |connection|
      puts 'Client connected.'
      loop do
        message = connection.recv(1024)
        break if message.to_s.empty?

        puts "Received: #{message.strip}"
        connection.puts(message)
      end
      puts 'Client disconnected.'
    ensure
      connection.close
    end
  end
rescue Interrupt
  puts "\nServer shutting down..."
ensure
  socket.close
end
