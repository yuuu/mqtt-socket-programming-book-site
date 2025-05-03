# frozen_string_literal: true

require 'socket'

def connect_tcp(host, port)
  socket = Socket.new(Socket::AF_INET, Socket::SOCK_STREAM, 0)
  sockaddr = Socket.sockaddr_in(port, host)
  socket.connect(sockaddr)
  socket
rescue SocketError => e
  puts 'ERR: ソケットの作成に失敗しました'
  raise e
rescue StandardError => e
  puts 'ERR: サーバーへの接続に失敗しました'
  raise e
end

def disconnect_tcp(socket)
  socket.close
rescue StandardError => e
  puts 'ERR: ソケットのクローズに失敗しました'
  raise e
end
