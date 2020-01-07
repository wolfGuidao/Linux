.PHONY:all
all:server client reader writer shm_server shm_client test_client test_server

test_server:test_server.c
	gcc $^ -o $@

test_client:test_client.c
	gcc $^ -o $@

shm_server:shm_server.c
	gcc $^ -o $@

shm_client:shm_client.c
	gcc $^ -o $@

server:server.c comm.c
	gcc $^ -o $@

client:client.c comm.c
	gcc $^ -o $@

reader:reader.c
	gcc $^ -o $@

writer:writer.c
	gcc $^ -o $@
