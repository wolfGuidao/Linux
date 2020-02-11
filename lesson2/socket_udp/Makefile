.PHONY:all
all:server client

server:dict_server.cc 
	g++ $^ -o $@ -std=c++11

client:dict_client.cc 
	g++ $^ -o $@ -std=c++11

.PHONY:clean
	rm server client
