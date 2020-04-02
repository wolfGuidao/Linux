LDFLAGS=-L /home/wudu/ThirdPartLibForCpp/el7.x86_64/third_part/lib
DEBUGFLAGS=-g
BIN=../bin/oj_server
CC=g++

.PHONY:$(BIN)
$(BIN):oj_server.cpp
	$(CC) $^ -o $@  $(DEBUGFLAGS) $(LDFLAGS) -lctemplate -ljsoncpp -lpthread

.PHONY:clean
clean:
	rm -f $(BIN)
