FLAG=-std=c++11 -L/usr/lib64/mysql -L/usr/include/jsoncpp/json -ljsoncpp -lmysqlclient -lpthread 

.PHONY:all
all:blog_server db_test

db_test:db_test.cc db.hpp
	g++ $^ -o $@ $(FLAG)

blog_server:blog_server.cc 
	g++ blog_server.cc -o blog_server $(FLAG)

.PHONY:clean
clean:
	rm  blog_server
