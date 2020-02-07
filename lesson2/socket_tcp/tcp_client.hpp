#pragma once
#include "tcp_socket.hpp"

class TcpClient 
{
  public:
    TcpClient(const std::string& ip, uint16_t port) : ip_(ip), port_(port) 
   {
      // [注意!!] 需要先创建好 socket
      sock_.Socket();
      //
    }

    ~TcpClient() 
    {
      sock_.Close();
    }

    bool Connect() 
    {
      return sock_.Connect(ip_, port_);
    }

    bool Recv(std::string* buf) 
    {
      return sock_.Recv(buf);
    }

    bool Send(const std::string& buf) 
    {
      return sock_.Send(buf);
    }

  private:
    TcpSocket sock_;
    std::string ip_;
    uint16_t port_;

};
