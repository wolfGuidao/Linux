#include"udp_server.hpp"


void Echo(const string&req,string* resp)
{
    *resp=req;
    printf("%s\n",req.c_str());

}


int main()
{
    UdpServer server;
    //lambad表达式
    //server.Start("0.0.0.0",9090,[](const string&req,string* resp){*resp=req;});
    server.Start("0.0.0.0",9090,Echo);
    return 0;
}
