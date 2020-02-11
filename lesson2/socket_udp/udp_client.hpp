#pragma once
#include "udp_socket.hpp"

class UdpClient
{
  public:
    UdpClient(const std::string& ip,uint16_t port)
      :ip_(ip)
       ,port_(port)
    {
      assert(sock_.Socket());
    }

    ~UdpClient()
    {
      sock_.Close();
    }

    bool RecvFrom(std::string* buf)
    {
      return sock_.RecvFrom(buf);
    }

    bool Sendto(const std::string& buf)
    {
      return sock_.SendTo(buf,ip_,port_);
    }

  private:
    UdpSocket sock_;
    
    //服务器端ip、port
    std::string ip_;
    uint16_t port_;
};
