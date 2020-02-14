#目标对象（要生成的可执行程序或者动态库或者静态库）
#依赖对象（生成目标对象的时候所依赖的一些文件，.o .c）
#编译命令
#all:out aa
#
#out:test.o
#	gcc test.o -o out
#
#out.o:test.c
#	gcc -c test.c -o out.o
#
#aa:a.c
#	gcc a.c -o aa
#.PHONY:out
#out:test.c
#	gcc test.c -o out
#
#.PHONY:clean
#clean:
#	rm out
#预定义变量
# $^ : 所有的依赖对象
# $@ : 目标对象
# $< : 依赖的第一个对象

#out:test.o a.o
#	gcc $^ -o $@
##
##xxx.o --> test.o ==> %.o %=test ==> test.c
##a.o ==> %=a a.c
#%.o:%.c
#	gcc -c $< -o $@

#gcc test.o a.o -o out
#gcc test.c a.c c-o out

bin=out
src=$(wildcard *.c)
#src=test.c a.c
obj=$(patsubst %.c, %.o, $(src))
#obj=test.o a.o
Cflags=-g
Include=-I ..

$(bin):$(obj)
	gcc $^ -o $@ 
%.o:%.c
	gcc $(Cflags) $(Include) -c $< -o $@
clean:
	rm $(obj) $(bin)
