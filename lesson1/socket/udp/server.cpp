#include<iostream>
#include<unistd.h>
#include<cstring>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;


//实现UDP版本的服务器

int main()
{

    //1.创建一个socket
    //int socket(int domain,int type,int protocol)
    //domain:AF_INET是一个宏，表示使用ipv4协议
    //type:SOCK_DGRAM表示使用UDP协议
    //第三个参数默认为0
    int sock=socket(AF_INET,SOCK_DGRAM,0);
    if(sock<0)
    {
        perror("socket");
        return 1;
    }
    

    //2.把当前的socket绑定一个ip+端口号
    //int bind(int sockfd,const struct sockaddr* addr,socklen_t addrlen);
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    //inet_addr的作用是把点分十进制转成整数,ip地址本应该先转为网络字节序，但函数内置了，无需自己转
    //全0ip表示关联整个机器所有网卡
    addr.sin_addr.s_addr=inet_addr("0.0.0.0");
    //端口号必须的转为网络字节序
    addr.sin_port=htons(9090);
    int ret=bind(sock,(sockaddr*)&addr,sizeof(addr));
    if(ret<0)
    {
        perror("bind");
        return 1;
    }
    printf("有客户端进入链接\n");

    //3.处理服务器收到的请求
    while(true)
    {
        //a.读取客户端的请求
        
        //面向数据报的函数接口recvfrom
        //sockfd:文件;buf:缓冲区;size大小;flags:0;addr:客户端的ip、端口号地址；addrlen：客户端的长度;
        sockaddr_in peer;//输出型参数,表示客户端的IP、端口相关信息
        socklen_t len=sizeof(peer); //输入输出型参数
        char buf[1024]={0};
        size_t n=recvfrom(sock,buf,sizeof(buf)-1,0,(sockaddr*)&peer,&len);
        if(n<0)
        {
            perror("recvfrom");
            continue;
        }
        buf[n]='\0';


        //b.根据请求计算响应
        
        //回显服务器,此处省略

        //c把响应发给客户端
        
        n = sendto(sock,buf,strlen(buf),0,(sockaddr*)&peer,len);
        if(n<0)
        {
            perror("sento");
            continue;
        }
        //打印时应把整形形式的ip转为点分十进制形式；把端口号peer的网络字节序转换为主机字节序
        printf("[%s:%d] buf: %s\n",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port),buf);
    }
    close(sock);

    return 0;
}
