.PHONY:all
all: server client

server:calc_server.cpp  tcp_socket.hpp tcp_thread_server.hpp
	g++ calc_server.cpp -o server -std=c++11 -lpthread -ljsoncpp

client:calc_client.cpp  tcp_socket.hpp tcp_client.hpp
	g++ calc_client.cpp -o client -std=c++11 -lpthread -ljsoncpp

.PHONY:clean
clean:
	rm client server
