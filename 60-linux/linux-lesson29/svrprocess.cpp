#include "tcpsvr.hpp"
#include <signal.h>
#include <sys/wait.h>

void sigcb(int signo)
{
    (void)signo;
    while(1)
    {
        waitpid(-1, NULL, WNOHANG);
    }
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("./svrprocess [ip] [port]\n");
        return 0;
    }

    signal(SIGCHLD, sigcb);
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

    while(1)
    {
        //接收连接
        TcpServer peerts;
        struct sockaddr_in peeraddr;
        if(!ts.Accept(peerts, &peeraddr))
        {
            continue;
        }
        printf("Have a new connect %s:%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
        //创建子进程
        int pid = fork();
        if(pid < 0)
        {
            perror("fork");
            exit(1);
        }
        else if(pid == 0)
        {
            //child
            //子进程对客户端进行1对1服务
            while(1)
            {
                std::string buf;
                peerts.Recv(buf);
                printf("cli say:%s\n", buf.c_str());

                printf("svr say:");
                fflush(stdout);
                std::cin >> buf;

                peerts.Send(buf);
            }
            peerts.Close();
            exit(1);
        }
        //父进程逻辑
        peerts.Close();
    }
    ts.Close();
    return 0;
}
