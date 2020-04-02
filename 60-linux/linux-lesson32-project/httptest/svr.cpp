#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "tcpsvr.hpp"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("./svr [ip] [port]\n");
        return 0;
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpServer ts;
    if(!ts.CreateSock())
    {
        return 0;
    }

    if(!ts.Bind(ip, port))
    {
        return 0;
    }

    if(!ts.Listen(5))
    {
        return 0;
    }
    
    //想使用TcpServer当中的收发数据接口，并且使用成员变量保存新创建(接收和发送数据)出来的socket
    while(1)
    {
        TcpServer peerts;
        if(!ts.Accept(peerts))
        {
            return 0;
        }

        std::string buf;
        peerts.Recv(buf);
        printf("chrome send Data:%s\n", buf.c_str());

        //http协议去组织 数据格式
        //响应首行
        //响应头部
        //空行
        //正文
        //std::string body = "<html><body><h1>hello world</h1></body></html>";
        //char arraybuf[10240] = {0};
        //sprintf(arraybuf, "%s %s %s\r\nContent-Length: %lu\r\nContent-Type: %s\r\n\r\n","HTTP/1.1", "200", "OK", body.size(), "text/html");
        //std::string header;
        //header.assign(arraybuf, strlen(arraybuf));

        //std::string body = "<html><body><h1>hello world</h1></body></html>";
        //std::stringstream ss;
        //ss << "HTTP/1.1 200 OK\r\n";
        //ss << "Content-Length: " << body.size() << "\r\n";
        //ss << "Content-Type: text/html\r\n";
        //ss << "\r\n";
        //std::string header = ss.str();

        //std::string body = "<html><body><h1>hello world</h1></body></html>";
        //std::stringstream ss;
        //ss << "HTTP/1.1 302 Found\r\n";
        //ss << "Content-Length: " << body.size() << "\r\n";
        //ss << "Content-Type: text/html\r\n";
        //ss << "Location: https://www.baidu.com\r\n";
        //ss << "\r\n";

        //std::string body = "<html><body><h1>hello world</h1></body></html>";
        //std::stringstream ss;
        //ss << "HTTP/1.1 404 Page Not Fuond\r\n";
        //ss << "Content-Length: " << body.size() << "\r\n";
        //ss << "Content-Type: text/html\r\n";
        //ss << "\r\n";

        std::string body = "<html><body><h1>hello world</h1></body></html>";
        std::stringstream ss;
        ss << "HTTP/1.1 502 Bad Gateway\r\n";
        ss << "Content-Length: " << body.size() << "\r\n";
        ss << "Content-Type: text/html\r\n";
        ss << "\r\n";
        std::string header = ss.str();
        peerts.Send(header);
        peerts.Send(body);
        peerts.Close();
    }
    ts.Close();
    return 0;
}
