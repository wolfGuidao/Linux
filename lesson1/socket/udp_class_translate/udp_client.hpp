#pragma once
#include"udp_socket.hpp"

class UdpClient
{
public:
    UdpClient(const string& ip,uint16_t port)
    :servere_ip_(ip)
     ,server_port_(port)
    {
        sock_.Socket();
    }

    ~UdpClient()
    {
        sock_.Close();
    }

    bool RecvFrom(string*msg)
    {
        return sock_.RecvFrom(msg);
    }

    bool SendTo(const string& msg)
    {

        return sock_.SendTo(msg,servere_ip_,server_port_);
    }
private:
    UdpSocket sock_;
    string servere_ip_;
    uint16_t server_port_;
};
