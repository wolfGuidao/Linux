.PHONY:all
all: server client

server:tcp_thread_server.cpp tcp_server.hpp tcp_socket.hpp tcp_thread_server.hpp
	g++ tcp_thread_server.cpp -o server -std=c++11 -lpthread

.PHONY:clean
clean:
	rm client server
