#include "tcp_client.hpp"
#include <iostream>

int main(int argc, char* argv[]) 
{
  if (argc != 3) 
  {
    printf("Usage ./dict_client [ip] [port]\n");
    return 1;
  }

  TcpClient client(argv[1], atoi(argv[2]));
  bool ret = client.Connect();
  if (!ret) 
  {
    return 1;
  }

  for (;;) 
  {
    std::cout << "请输入要查询的单词:" << std::endl;
    std::string word;
    std::cin >> word;

    client.Send(word);
    std::string result;
    client.Recv(&result);
    std::cout <<word<<" 意思是："<< result << std::endl;
  }
return 0;
}
