#include<iostream>
#include<unistd.h>
#include<string>
#include<cstring>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;

//实现客户端程序

//通过命令行参数传递服务器端ip：
int main(int argc,char* argv[])
{
    //1.创建一个sorcket
    int sock=socket(AF_INET,SOCK_DGRAM,0);
    if(sock<0)
    {
        perror("sock");
        return 1;
    }
    //2.客户端一般不需要bind
    //bind意味着和某个端口号绑定关联在一起
    //如果客户端没有bind，操作系统会随机分配一个端口号
    //但如果是服务器端不bind，那么服务器每次启动，端口号发生变化，客户端就无法链接了
    //如果客户端bind,注意通常情况下一个端口号只能被一个进程bind，在此情况下，如果其他客户端同样bind到同一个
    //端口号，就会出错，所以通常情况下，由系统给客户端分配端口号
    
    //3.准备服务器端的sockaddr_in结构
    sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr(argv[1]);
    server_addr.sin_port=htons(9090);

    //4.客户端直接发送数据
    while(true)
    {
        char buf[1024];
        printf("请输入一段内容:\n");
        fflush(stdout);
        scanf("%s",buf);
        sendto(sock,buf,strlen(buf),0,(sockaddr*)&server_addr,sizeof(server_addr));

        //5.从服务器接收返回结果
        char buf_output[1024]={0};
        recvfrom(sock,buf_output,sizeof(buf_output)-1,0,NULL,NULL);//为空表示不关注对端地址
        printf("server resp: %s\n",buf_output);
    }
    return 0;
}
