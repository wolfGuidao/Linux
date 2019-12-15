#pragma once
#include<iostream>
#include<cstring>
#include<stdio.h>
#include<string>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;

//封装UDP
class UdpSocket
{
public:
    UdpSocket()
    :fd_(-1)
    {

    }

    //创建一个udp的socket，返回true表示创建成功，false表示创建失败
    bool Socket()
    {
        fd_=socket(AF_INET,SOCK_DGRAM,0);
        if(fd_<0)
        {
            perror("socket");
            return false;
        }
        
        return true;
    }

    //关闭socket
    bool Close()
    {
        if(fd_!=-1)
        {
            close(fd_);
        }
        return true;
    }

    //bind函数,ip为点分十进制的ip，port为端口号
    bool Bind(const string& ip,uint16_t port)
    {
        //根据参数构造一个sockaddr_in结构
        //调用bind
        sockaddr_in addr;
        addr.sin_family=AF_INET;
        addr.sin_addr.s_addr=inet_addr(ip.c_str());
        addr.sin_port=htons(port);
        int ret=bind(fd_,(sockaddr*)&addr,sizeof(addr));
        if(ret<0)
        {
            perror("bind");
            return false;
        }
        return true;
    }

    //recvfrom函数,除了成功还要返回什么？注意都是输出型参数
    //1.读到的数据msg
    //2.对方的ip地址
    //3.对方的端口号
    bool RecvFrom(string* msg,string* ip=NULL,uint16_t* port=NULL)
    {
       char buf[1024*10]={0};
       sockaddr_in peer;
       socklen_t len=sizeof(peer);
       size_t n = recvfrom(fd_,buf,sizeof(buf)-1,0,(sockaddr*)&peer,&len);
       if(n<0)
       {
           perror("recvfrom");
           return false;
       }
       *msg=buf;
       if(ip!=NULL)
       {
           *ip=inet_ntoa(peer.sin_addr);
       }
       if(port!=NULL)
       {
           *port=ntohs(peer.sin_port);
       }
        return true;
    }

    //sendto函数
    bool SendTo(const string& msg,const string& ip,uint16_t port )
    {
        sockaddr_in addr;
        addr.sin_family=AF_INET;
        addr.sin_addr.s_addr=inet_addr(ip.c_str());
        addr.sin_port=htons(port);
        size_t n=sendto(fd_,msg.c_str(),msg.size(),0,(sockaddr*)&addr,sizeof(addr));
        if(n<0)
        {
            perror("sendto");
            return false;
        }
        return true;
    }
private:
    //模拟表示socket
    int fd_;
};

