.PHONY:all
all: server client

server:dict_tcp_server.cpp tcp_server.hpp tcp_socket.hpp
	g++ dict_tcp_server.cpp -o server -std=c++11

client:dict_tcp_client.cpp tcp_client.hpp tcp_socket.hpp
	g++ dict_tcp_client.cpp -o client -std=c++11

.PHONY:clean
clean:
	rm client server
