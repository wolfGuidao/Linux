.PHONY:all
all:server client

server:echo_server.cpp udp_server.hpp
	g++ -g echo_server.cpp -o server

client:echo_client.cpp udp_client.hpp
	g++ -g echo_client.cpp -o client
