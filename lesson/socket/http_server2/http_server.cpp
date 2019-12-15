#include"tcp_thread_server.hpp"

//状态码为：302 Found
void HttpProcess(const string& req,string* resp)
{
    //无论用户的请求是什么，都返回hello world
    (void)req;
    string first_line="HTTP/1.1 302 Found\n";
    string header= "Location:http://www.baidu.com\n"
        "Content-Type:text/html\n"
        "Content-Length:10\n";
    string body="李向阳是个憨憨哈哈哈\n";
    *resp=first_line + header + "\n" + body;
}

int main()
{
    TcpThreadServer server;
    server.Start("0.0.0.0",9090,HttpProcess);
    return 0;
}
