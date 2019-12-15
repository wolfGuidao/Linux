#include"tcp_thread_server.hpp"

//状态码为：200 ok
void HttpProcess(const string& req,string* resp)
{
    //无论用户的请求是什么，都返回hello world
    (void)req;
    string first_line="HTTP/1.1 200 ok\n";
    string body="<html><div><李向阳是个沙雕</div></html>";
    string header = "Content-Type: text/html\nContent-Length:"+to_string(body.size())+"\n";
    *resp=first_line + header + "\n" + body;
}

int main()
{
    TcpThreadServer server;
    server.Start("0.0.0.0",9090,HttpProcess);
    return 0;
}
