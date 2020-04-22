#include "tcpsvr.hpp"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("./cli [ip] [port]\n");
        return 0;
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpServer ts;
    if(!ts.CreateSock())
    {
        return 0;
    }

    if(!ts.Connect(ip, port))
    {
        return 0;
    }

    while(1)
    {
        std::string name;
        std::string address;
        //printf("cli say name:");
        //fflush(stdout);
        //std::cin >> name;
        printf("cli say address:");
        fflush(stdout);
        std::cin >> address;

        CliInfo cli_info;
        //不安全的原因在于 无法掌控name.c_str()大小
            //name += i;
        strcpy(cli_info.Name_, "1");
        strcpy(cli_info.Address_, address.c_str());
        ts.Send(cli_info);

        strcpy(cli_info.Name_, "2");
        strcpy(cli_info.Address_, address.c_str());
        ts.Send(cli_info);

        strcpy(cli_info.Name_, "3");
        strcpy(cli_info.Address_, address.c_str());
        ts.Send(cli_info);

        ts.Recv(&name, &address);
        printf("svr say name is : %s, svr say address is %s\n", name.c_str(), address.c_str());
    }
    ts.Close();
    return 0;
}
