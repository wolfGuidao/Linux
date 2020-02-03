#pragma once
#include<pthread.h>
#include<functional>
#include"tcp_socket.hpp"

typedef function<void(const string&,string*)>Handler;
#define CHECK_RET(exp)if(!(exp)){\
    return false;\
}

class TcpThreadServer
{
    public:
        TcpThreadServer()
        {

        }

        ~TcpThreadServer()
        {
            listen_sock.Close();
        }

        bool Start(const string& ip,uint16_t port,Handler handler)
        {
            printf("server is ok!\n");
            //1.创建socket
            CHECK_RET(listen_sock.Socket());

            //2.绑定端口号
            CHECK_RET(listen_sock.Bind(ip,port));

            //3.监听
            CHECK_RET(listen_sock.Listen());

            //4.进入主循环
            
            while(1)
            {
            //5.调用accept
                TcpSocket client_sock;
                string peer_ip;
                uint16_t peer_port;
                bool ret = listen_sock.Accept(&client_sock,&peer_ip,&peer_port);
                if(!ret)
                {
                    continue;
                }
                printf("[%s:%d]客户端建立链接！\n",peer_ip.c_str(),peer_port);

            //6.创建线程处理客户端逻辑
            PthreadConnect(client_sock,peer_ip,peer_port,handler);
            
            }
        }
    private:
        TcpSocket listen_sock;

        struct ThreadEntryArg
        {
            TcpSocket client_sock;
            string ip;
            uint16_t port;
            Handler handler;
        };

        void PthreadConnect(TcpSocket& client_sock,const string& ip,uint16_t port,Handler handler)
        {
            //1.创建线程
            pthread_t tid ;

            ThreadEntryArg* arg = new ThreadEntryArg;
            arg->client_sock = client_sock;
            arg->ip = ip;
            arg->port = port;
            arg->handler = handler;
            pthread_create(&tid,NULL,ThreadEntry,(void*)arg);

            //2.主线程让函数直接返回
            pthread_detach(tid);
            
            //3.新线程循环处理客户端的操作：
            
        }

        static void* ThreadEntry(void* arg)
        {
            ThreadEntryArg* argument = (ThreadEntryArg*)arg;

            TcpSocket client_sock = argument->client_sock;
            string& ip=argument->ip;
            uint16_t port=argument->port;
            Handler handler=argument->handler;

            while(1)
            {
            //   a)读取客户端请求
                string req;

                int ret=client_sock.Recv(&req);
                if(ret<0)
                {
                    continue;
                }

                if(ret==0)
                {
                    client_sock.Close();
                    printf("[%s:%d],客户端断开链接了\n！",ip.c_str(),port);
                    break;
                }

            //   b)根据请求计算响应
                printf("[%s:%d],客户端请求：%s \n！",ip.c_str(),port,req.c_str());
                string resp;
                handler(req,&resp);

            //   c)把响应写回客户端
                 client_sock.Send(resp);

            }
            delete argument;
            return NULL;
        }
};
