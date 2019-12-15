#pragma once
//通用的TCP服务器

#include"tcp_socket.hpp"
#include<functional>
typedef function<void(const string&,string*)> Handler;
#define CHECK_RET(exp) if(!(exp)){\
    return  false;\
}

class TcpServer
{
    public:
        TcpServer()
        {

        }

        bool Start(const string& ip,uint16_t port,Handler handler)
        {
            //1.创建一个socket
            CHECK_RET(listen_socket_.Socket());

            //2.绑定端口号
            CHECK_RET(listen_socket_.Bind(ip,port));

            //3.进行监听
            CHECK_RET(listen_socket_.Listen());
            printf("Server Start is ok!\n");

            //4.进入主循环
            while(1)
            {
                //5.通过Accept获取一个连接,获取对端的IP和端口号，即输出型参数
                TcpSocket client_sock;
                string ip;
                uint16_t port;

                //核心问题是，第一次Accept之后就进入了一个循环，在这个过程中，循环一直没有结束，Accept
                //没有被重复调用，后续链接来的客户端都在内核链接队列中，一直得不到处理，
                //所以因让程序更快的调用到Accept，即采用多进程或多线程
                bool ret = listen_socket_.Accept(&listen_socket_,&ip,&port);
                if(!ret)
                {
                    continue;
                }
                printf("[%s:%d],有客户端在进行链接!\n",ip.c_str(),port);

                //6.和客户端进行具体的沟通,一次连接中要和客户端进行多次沟通
                while(1)
                {
                    //a).读取请求
                    string req;
                    int r = client_sock.Recv(&req);
                    if(r<0)
                    {
                        continue;
                    }
                    if(r==0)
                    {
                        //对端关闭了socket
                        client_sock.Close();
                        printf("[%s,%d]对端关闭了链接!\n",ip.c_str(),port);
                        break;
                    }
                    printf("[%s:%d],客户端发送了：%s !\n",ip.c_str(),port,req.c_str());

                    //b).根据请求计算响应
                    string resp;
                    handler(req,&resp);
                    //c).把响应写回客户端
                    client_sock.Send(resp);
                }
            }
        }
    private:
        TcpSocket listen_socket_;
};

