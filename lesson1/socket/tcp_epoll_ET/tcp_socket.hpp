#pragma once
#include<fcntl.h>
#include<iostream>
#include<stdio.h>
#include<string>
#include<unistd.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;

class TcpSocket
{
public:
    TcpSocket()
    :fd_(-1)
    {
        
    }

    TcpSocket(int fd)
    :fd_(fd)
    {

    }

    bool Socket()
    {
        //SOCK_STREAM代表tcp，是面向字节流的
        fd_=socket(AF_INET,SOCK_STREAM,0);
        if(fd_<0)
        {
            return false;
        }
        return true;
    }

    //给服务器使用的
    bool Bind(const string& ip,uint16_t port)
    {
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

    //进入监听状态int listen(int sockfd,int backlog);
    //scokfd:fd_; backlog:表示操作系统内核队列里已经创建好的连接的个数，即队列长度
    //给服务器使用的
    bool Listen()
    {
        int ret=listen(fd_,30);
        if(ret<0)
        {
            perror("listen");
            return false;
        }
        return true;
    }

    //把连接队列中的链接取出一个到户代码中,如果队列中没有连接，默认就会阻塞
    //int accept(int sockfd,struct sockaddr* addr,socklen_t *addrle),addrle:是一个输入输出型参数
    //addr:对端IP地址和端口号;addrle:是一个输入输出型参数;返回值也是一个socket
    //给服务器使用的
    bool Accept(TcpSocket* peer,string* ip=NULL,uint16_t* port=NULL)
    {
        sockaddr_in peer_addr;
        socklen_t len=sizeof(peer_addr);

        //client_sock:也是一个socket
        int client_sock = accept(fd_,(sockaddr*)&peer_addr,&len);
        if(client_sock<0)
        {
            perror("client_sock");
            return false;
        }
        //输出型参数peer，ip,port,方便客户端使用，把对端的fd,IP,port情况返回
        peer->fd_=client_sock;
        if(ip!=NULL)
        {
            *ip=inet_ntoa(peer_addr.sin_addr);
        }
        if(port!=NULL)
        {
            *port=ntohs(peer_addr.sin_port);
        }
        return true;
    }


    //给服务器和客户端使用的
    int Recv(string* msg)
    {
        msg->clear();
        char buf[1024*10]={0};
        //recv的返回值：读取成功，返回结果为读到的字节数；如果读取失败，返回结果为-1，如果对端关闭了socket，返回结果为0；
        ssize_t n = recv(fd_,buf,sizeof(buf)-1,0);
        if(n<0)
        {
            perror("recv");
            return -1;
        }
        else if(n==0)
        {
            //需要考虑返回0的情况
            return 0;
        }
        else
        {
            //把读到buf中的内容复值到msg中
            msg->assign(buf);
            return 1;
        }
    }

    //给服务器和客户端使用的
    bool Send(const string& msg)
    {
        ssize_t n= send(fd_,msg.c_str(),msg.size(),0);
        if(n<0)
        {
            perror("send");
            return false;
        }
        return true;
    }

    //给客户端使用的
    bool Connect(const string& ip,uint16_t port)
    {
        sockaddr_in addr;
        addr.sin_family=AF_INET;
        addr.sin_addr.s_addr=inet_addr(ip.c_str());
        addr.sin_port=htons(port);

        int ret=connect(fd_,(sockaddr*)&addr,sizeof(addr));
        if(ret<0)
        {
            perror("connect");
            return false;
        }
        return true;
    }

    bool Close()
    {
        if(fd_!=-1)
        {
            close(fd_);
        }
        return true;
    }

    int GetFd()const
    {
        return fd_;
    }

    bool SetNoBlock()
    {
        //获取到fd_文件描述符的相关标志位，保存在f1这个位图中
        int f1=fcntl(fd_,F_GETFL);
        if(f1<0)
        {
            perror("funcl F_GETFL");
            return false;
        }
        int ret = fcntl(fd_,F_SETFL,f1|O_NONBLOCK);
        if(ret<0)
        {
            perror("fcnl F_SETFL");
            return false;
        }
        return true;
    }

    int RecvNoBlock(string* msg)
    {
        msg->clear();
        //这是一个近似的写法，没有考虑粘包问题
        //如果tcp的接受缓冲 区为空，此时读不到数据，但recv同样会返回，如果没读到->errno会被设为EAGAIN/EWOULDBLOCK
        //如果读到的数据长度比设定的用户缓冲区长度一样，就继续往下读
        //如果当前读到的数据的长度已经比缓冲区长度短，就认为读完了
        char temp[1024*10];
        while(1)
        {
            ssize_t n=recv(fd_,temp,1024*10-1,0);
            if(n<0)
            {
                if(errno==EAGAIN||errno==EWOULDBLOCK)
                {
                    //表明缓冲区已经没有数据了,但仍还要继续尝试读
                    continue;
                }
                else
                {
                    //其他原因导致的读失败，就退出函数
                    return -1;
                }
            }
            else if(n==0)
            {
                printf("对端关闭链接\n");
                return 0;
            }
                temp[n]='\0';
                //把这个数据追加到输出参数中
                msg->append(temp);
                if(n<(ssize_t)sizeof(temp)-1)
                {
                    //这次读取没有填充完整个缓冲区，认为当前都缓冲区中没有数据了
                    break;
                }
        }
        return msg->size();
    }

    bool SendNoBlock(const string& msg)const
    {
        //当实际发送的数据长度和msg一样就代表发送完了
        //send(fd_,msg.c_str(),msg.size(),0);//有风险，由于当前是非阻塞IO，如果发送缓冲区满了，函数一调用就返回可能会出现发送失败的情况
        //所以最好采用小块小块的发送，每发送完一个数据后，都检查自己发送了多少个数据，累加发送的数据的长度
        //当长度等于msg的长度时才代表最终发送完成
        
        //记录当前写到的位置
       ssize_t cur_pos=0;
       ssize_t left_size =msg.size();
       while(1)
       {
           ssize_t write_size=send(fd_,msg.data()+cur_pos,left_size,0);
           if(write_size<0)
           {
               if(errno==EAGAIN||errno==EWOULDBLOCK)
               {
                   continue;
               }
               return false;
           }
           cur_pos+=write_size;
           left_size-=write_size;
           if(left_size<=0)
           {
               break;
           }
       }
        return true;

    }

private:
int fd_;
};
