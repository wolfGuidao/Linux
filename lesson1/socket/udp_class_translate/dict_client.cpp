#include"udp_client.hpp"

int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        printf("./client [ip]\n");
    }
    UdpClient client(argv[1],9090);
    
    string buf;
    printf("请输入你要查询的单词：\n");
    cin>>buf;
    client.SendTo(buf);

    string out_buf;
    client.RecvFrom(&out_buf);

    printf("resp:%s\n",out_buf.c_str());
}
