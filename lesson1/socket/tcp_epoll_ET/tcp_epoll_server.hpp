#pragma once
#include<sys/epoll.h>
#include<vector>
#include "tcp_socket.hpp"

//相等于interface
class Base
{
    public:
        virtual void Add(const TcpSocket&)=0;
        virtual void Del(const TcpSocket&)=0;
        virtual void Wait(vector<TcpSocket>* output) = 0;
};

class Epoll
{
    public:
        Epoll()
        {
            //创建一个epoll对象
            epoll_fd_=epoll_create(100);
        }

        ~Epoll()
        {
            close(epoll_fd_);
        }

        void Add(const TcpSocket&sock,bool is_et=false)
        {
            printf("[Epoll::Add]%d\n",sock.GetFd());

            //构造event参数
            epoll_event event;
            event.events=EPOLLIN;
            if(is_et)
            {
                event.events|=EPOLLET;
            }

            event.data.fd=sock.GetFd();
            //sock.GetFd()是键，event是值，注意这里键和值中都有sock，原因在下面解释
            epoll_ctl(epoll_fd_,EPOLL_CTL_ADD,sock.GetFd(),&event);
        }

        void Del(const TcpSocket& sock)
        {
            printf("[Epoll::del]%d\n",sock.GetFd());

            epoll_ctl(epoll_fd_,EPOLL_CTL_DEL,sock.GetFd(),NULL);


        }

        //输出型参数output，告诉我们有多少个文件描述符处于就绪状态
        void Wait(vector<TcpSocket>* output)
        {
            output->clear();
            //等待文件描述分就绪，epollWait

            epoll_event events[100];
            //-1表示阻塞等待
            //该函数返回之后，就会有若干个文件描述符就绪，保存在events数组中，返回值代表数组中具体有多少个文件描述符就绪
            //epoll_wait返回的内容是键值对的值，如果不加处理，用户就不知道对应的文件描述符是谁，所以再插入的时候把sock往值里面也插入一份
            int nfds=epoll_wait(epoll_fd_,events,100,-1);
            if(nfds<0)
            {
                perror("epoll_wait");
                return ;
            }
            //以此处理就绪的文件描述符
            for(int i=0;i<nfds;i++)
            {
                int sock = events[i].data.fd;
                output->push_back(TcpSocket(sock));
            }

        }
    private:
        int epoll_fd_;//通过epoll_fd_就能找到内核中的对象，从而进行文件描述符管理
};


//用epoll实现tcp_server
#include<functional>

typedef function<void(const string&,string*)>Handler;
#define CHECK_RET(exp)if(!(exp)){return false;}

class TcpEpollServer
{
    public:
        bool Start(const string& ip,uint16_t port,Handler handler)
        {
            //1.创建socke   
            TcpSocket listen_sock;
            CHECK_RET(listen_sock.Socket());

            //2.绑定socket
            CHECK_RET(listen_sock.Bind(ip,port));

            //3.监听socket
            CHECK_RET(listen_sock.Listen());

            //4.创建epoll对象，并把listen_sock用epoll管理起来
            Epoll epoll;
            epoll.Add(listen_sock);

            //5.进入主循环
            while(1)
            {
                //6.循环内部使用Epoll::wait方法等待文件描述符就绪
                vector<TcpSocket>output;
                epoll.Wait(&output);

                //7.循环处理每个就绪的socket，处理结果分两种
                //  a):listen_sock就调用Accept
                //  b):其他调用Recv
                for(auto sock: output)
                {
                    if(sock.GetFd()==listen_sock.GetFd())
                    {
                        TcpSocket client_sock;
                        sock.Accept(&client_sock);
                        epoll.Add(client_sock);
                    }
                    else
                    {
                        string req;
                        //默认情况下epoll是阻塞的LT模式
                        int n = sock.RecvNoBlock(&req);
                        if(n<0)
                        {
                            continue;
                        }
                        else if(n==0)
                        {
                            //对端关闭链接
                            sock.Close();
                            epoll.Del(sock);
                            printf("[client %d]对端关闭了链接\n",sock.GetFd());
                            continue;
                        }
                        printf("[client %d]%s\n",sock.GetFd(),req.c_str());
                        string resp;
                        handler(req,&resp);
                        sock.SendNoBlock(resp);
                    }
                }


            }
        }
    private:

};
