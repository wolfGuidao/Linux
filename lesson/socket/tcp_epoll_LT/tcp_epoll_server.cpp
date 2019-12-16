//#include"tcp_server.hpp"
//#include"tcp_select_server.hpp"
#include"tcp_epoll_server.hpp"
#include<unordered_map>
#include<iostream>
using namespace std;

void Handler1(const string& req,string* resp)
{
    *resp =req;
}

int main()
{
    TcpEpollServer server;
    server.Start("0.0.0.0",9090,Handler1);
    return 0;
}
