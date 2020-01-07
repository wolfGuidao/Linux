.PHONY:all
all:f_to_pipe pipe_to_f server client

server:server.c
	gcc $^ -o $@

client:client.c
	gcc $^ -o $@

f_to_pipe:f_to_pipe.c
	gcc $^ -o $@

pipe_to_f:pipe_to_f.c
	gcc $^ -o $@
