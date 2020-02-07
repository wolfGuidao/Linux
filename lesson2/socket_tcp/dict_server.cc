#include <unordered_map>
#include "tcp_server.hpp"

std::unordered_map<std::string, std::string> g_dict;

void Translate(const std::string& req, std::string* resp) 
{
  auto it = g_dict.find(req);
  if (it == g_dict.end()) 
  {
    *resp = "未找到";
    return;
  }

  *resp = it->second;
  return;
}

#if 0
int main(int argc, char* argv[]) 
{
  if (argc != 3) 
  {
    printf("Usage ./dict_server [ip] [port]\n");
    return 1;
  }
  
  // 1. 初始化词典
   g_dict.insert(std::make_pair("hello", "你好"));
   g_dict.insert(std::make_pair("world", "世界"));
   g_dict.insert(std::make_pair("bit", "贼NB"));

   // 2. 启动服务器
    TcpServer server(argv[1], atoi(argv[2]));
    server.Start(Translate);

    return 0;
   //
}
#endif

int main() 
{
  
  // 1. 初始化词典
   g_dict.insert(std::make_pair("hello", "你好"));
   g_dict.insert(std::make_pair("world", "世界"));
   g_dict.insert(std::make_pair("bit", "贼NB"));

   // 2. 启动服务器
    TcpServer server("0.0.0.0",9090);
    server.Start(Translate);

    return 0;
   //
}
