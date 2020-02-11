#include "udp_server.hpp"
#include <iostream>
#include <unordered_map>

std::unordered_map<std::string,std::string> g_dict;

void Translate(const std::string& req,std::string* resp)
{
  auto it = g_dict.find(req);
  if(it == g_dict.end())
  {
    *resp = "未查到";
    return;
  }
  *resp = it->second;
}

#if 0
int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    printf("./  ip  ,port\n");
    return 1;
  }

  g_dict.insert(std::make_pair("hello", "你好"));
  g_dict.insert(std::make_pair("world", "世界"));
  g_dict.insert(std::make_pair("c++", "最好的编程语言"));
  g_dict.insert(std::make_pair("bit", "特别NB"));

  UdpServer server;
  server.Start(argv[1],atoi(argv[2]),Translate);

  return 0;
}
#endif

int main()
{
  g_dict.insert(std::make_pair("world", "世界"));
  g_dict.insert(std::make_pair("c++", "最好的编程语言"));
  g_dict.insert(std::make_pair("bit", "特别NB"));
  g_dict.insert(std::make_pair("hello", "您好"));

  UdpServer server;
  server.Start("0.0.0.0",9090,Translate);

  return 0;
}
