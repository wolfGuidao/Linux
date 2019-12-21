.PHONY:all
all:server client

server:dict_server.cpp udp_server.hpp udp_socket.hpp
	g++ dict_server.cpp -o server -std=c++11

client:dict_client.cpp udp_client.hpp udp_socket.hpp
	g++ dict_client.cpp -o client -std=c++11

