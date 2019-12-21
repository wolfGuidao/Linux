.PHONY:all
all: server client

server:tcp_process_server.cpp tcp_server.hpp tcp_socket.hpp tcp_process_server.hpp
	g++ tcp_process_server.cpp -o server -std=c++11

.PHONY:clean
clean:
	rm client server
