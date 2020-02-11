#include "udp_client.hpp"
#include <iostream>

int main(int argc, char* argv[]) 
{
  if (argc != 3) 
  {
    printf("Usage ./dict_client [ip] [port]\n");
    return 1;
  }

  UdpClient client(argv[1], atoi(argv[2]));

  for (;;)
  {
    std::string word;
    std::cout << "请输入您要查的单词: ";
    std::cin >> word;

    client.Sendto(word);

    std::string result;
    client.RecvFrom(&result);
    std::cout << word << " 意思是 " << result << std::endl;
  }
  return 0;
}
