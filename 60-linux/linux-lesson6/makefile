bin=out
src=$(wildcard *.cpp)
obj=$(patsubst %.cpp, %.o, $(src))

$(bin):$(obj)
	g++ $^ -o $@

%.o:%.cpp
	g++ -c -g $< -o $@

clean:
	rm $(bin) $(obj)
