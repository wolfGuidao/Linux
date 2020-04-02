#include "tcpsvr.hpp"
#include <boost/algorithm/string.hpp>
#include <vector>

int Sum(std::string Data)
{
    //1+1
    std::vector<std::string> output;
    std::string split_char = "+";
    boost::split(output, Data, boost::is_any_of(split_char), boost::token_compress_off);
    //int sum = atoi(output[0].c_str()) + atoi(output[1].c_str());
    int sum_number = 0;
    for(size_t i = 0; i < output.size(); i++)
    {
        sum_number += atoi(output[i].c_str());
    }
    return sum_number;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("./svr [ip] [port]\n");
        return 0;
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpServer ts;
    if(!ts.CreateSock())
    {
        return 0;
    }

    if(!ts.Bind(ip, port))
    {
        return 0;
    }

    if(!ts.Listen(5))
    {
        return 0;
    }
    TcpServer peerts;
    if(!ts.Accept(peerts))
    {
        return 0;
    }
    
    //想使用TcpServer当中的收发数据接口，并且使用成员变量保存新创建(接收和发送数据)出来的socket
    while(1)
    {
        sleep(15);
        Data data;
        peerts.Recv(data);
        //1+1
        printf("cli say:%d+%d\n", data.a, data.b);

        std::string buf;
        buf = std::to_string(data.a + data.b);
        peerts.Send(buf);
    }
    ts.Close();
    return 0;
}
