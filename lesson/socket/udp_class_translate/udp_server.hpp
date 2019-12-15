#pragma once
#include<assert.h>
#include"udp_socket.hpp"
#include<functional>

//通用UDP服务器类
//1.读取客户端发送的请求，
//2.根据请求计算响,这一步和相关业务有关，所以把2对应的代码提取成一个回调函数
//3.把响应写回客户端

typedef void (*Handler)(const string& req,string* resp);
//同时兼容函数指针和仿函数
//typedef function<void(const string&,string*)>Handler;

class UdpServer
{
public:
    UdpServer()
    {
        assert(sock_.Socket());
    }

    ~UdpServer()
    {
        sock_.Close();
    }

    //每次启动服务器时，指定服务器的IP和端口号及客户的业务函数
    bool Start(const string& ip,uint16_t port,Handler handler)
    {
        //1.服务器创建socket在构造内已经完成
        //2.绑定服务器端端口号
        bool ret=sock_.Bind(ip,port);
        if(!ret)
        {
            return false;
        }
        //3.进入死循环，处理客户端请求
        while(true)
        {
            //a.读取请求
            string req;
            string peer_ip;
            uint16_t peer_port;
            sock_.RecvFrom(&req,&peer_ip,&peer_port);
            //b.根据请求计算响应
            string resp;
            handler(req,&resp);
            //c.把响应返回客户端
            sock_.SendTo(resp,peer_ip,peer_port);
        }
    }
private:
UdpSocket sock_;
};
