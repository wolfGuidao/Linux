#pragma once
#include<signal.h>
#include<sys/wait.h>
#include<functional>
#include"tcp_socket.hpp"

//多进程版TCP服务器
typedef function<void(const string&,string*)>Handler;
#define CHECK_RET(exp)if(!(exp)){\
    return false;\
}

class TcpProcessServer
{
    public:
        TcpProcessServer()
        {

        }

        ~TcpProcessServer()
        {
            listen_sock_.Close();
        }

        bool Start(const string& ip,uint16_t port,Handler handler)
        {
            //在子进程结束后自动被回收，放止产生僵尸进程
            signal(SIGCLD,SIG_IGN);

            //1.创建socket
            CHECK_RET(listen_sock_.Socket());

            //2.绑定端口号
            CHECK_RET(listen_sock_.Bind(ip,port));

            //3.监听
            CHECK_RET(listen_sock_.Listen());

            //4.进入主循环
            while(1)
            {
                //5.调用Accept
                TcpSocket client_sock;
                string peer_ip;
                uint16_t peer_port;
                bool ret=listen_sock_.Accept(&client_sock,&peer_ip,&peer_port);
                if(!ret)
                {
                    continue;
                }
                printf("[%s:%d]有客户端建立链接!\n",peer_ip.c_str(),peer_port);

                //6.创建子进程，让子进程去处理客户端的请求，父进程继续调用Accept
                ProcessConnect(client_sock,peer_ip,peer_port,handler);
            }
        }
    private:
        TcpSocket listen_sock_;

        void ProcessConnect(TcpSocket& client_sock,string& ip,uint16_t port,Handler handler)
        {
            //1.创建子进程
            pid_t ret=fork();

            //2.父进程结束函数
            if(ret>0)
            {
                //父进程
                //注意父进程也必须关闭socket，否则就会出现文件描述符泄露
                client_sock.Close();

                //注意不能wait(NULL)，会阻塞,达不到效果
                return ;
            }

            //3.子进程循环做：
            while(1)
            {
                //      a)读取客户端请求
                //
                string req;
                int r =  client_sock.Recv(&req);
                if(r<0)
                {
                    continue;
                }
                if(r==0)
                {
                    printf("[%s:%d]客户端断开链接退出了!\n",ip.c_str(),port);
                    break;
                }
                printf("[%s:%d]客户端发送了：%s\n",ip.c_str(),port,req.c_str());

                //      b)根据请求计算响应
                string resp;
                handler(req,&resp);

                //      c)把响应写回客户端
                client_sock.Send(resp);
            }
            //子进程收尾工作
            //1.关闭socket
            client_sock.Close();

            //2.结束进程
            exit(0);
        }
};
