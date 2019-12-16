#include"udp_client.hpp"

int main()
{
    UdpClient client("192.168.221.136",9090);
    while(1)
    {
        char buf[1024*10];
        printf("请输入一段话：\n");
        scanf("%s",buf);
        int ret = client.SendTo(buf);
        if(ret<0)
        {
            continue;
        }
        string buff;
        client.RecvFrom(&buff);
        printf("buff:%s\n",buff.c_str());
    }
    return 0;
}
