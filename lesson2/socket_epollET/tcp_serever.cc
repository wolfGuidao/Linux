#include "tcp_epoll_server.hpp"
#include<unordered_map>

int main()
{
    TcpEpollServer server("0.0.0.0",9090);
    std::unordered_map<std::string,std::string>dict;
    dict.insert(std::make_pair("hello","你好"));
    dict.insert(std::make_pair("wolf ","爱你"));
    dict.insert(std::make_pair("guidao","哟"));
    dict.insert(std::make_pair("bite","比特"));
    dict.insert(std::make_pair("world","世界"));
    dict.insert(std::make_pair("feiwu","废物"));
    server.Start([&dict](const std::string& req,std::string* resp){
            auto it = dict.find(req);
            if(it==dict.end())
            {
              *resp="未找到";
            }
            else
            {
              *resp=it->second;
            }
            });
    return 0;
}
