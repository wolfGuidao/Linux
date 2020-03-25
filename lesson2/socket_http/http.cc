#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "httplib.h"

void Usage() 
{
  printf("usage: ./server [ip] [port]\n");

}

int main(int argc, char* argv[]) 
{
  if (argc != 3) 
  {
    Usage();
    return 1;

  }

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
  {
    perror("socket");
    return 1;

  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));

  int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
  if (ret < 0)
  {
    perror("bind");
    return 1;
  }

  ret = listen(fd, 10);
  if (ret < 0)
  {
    perror("listen");
    return 1;

  }

  for (;;) 
  {
    struct sockaddr_in client_addr;
    socklen_t len;

    int client_fd = accept(fd, (struct sockaddr*)&client_addr, &len);
    if (client_fd < 0)
    {
      perror("accept");
      continue;
    }

    char input_buf[1024 * 10] = {0}; // 用一个足够大的缓冲区直接把数据读完.

    ssize_t read_size = read(client_fd, input_buf, sizeof(input_buf) - 1);
    if (read_size < 0)
    {
      return 1;

    }

    printf("[Request] %s", input_buf);

    char buf[1024] = {0};

    const char* hello = "<h1>hello world</h1>";
    sprintf(buf,"Http/1.0 200 ok\nContent-Length:%lu\n\n%s", strlen(hello), hello);
    write(client_fd, buf, strlen(buf));
  }
  return 0;

}

#if 0

int main()
{
  using namespace httplib;
  Server ser;
  ser.Get("/",[](const Request& req,Response& res)
      {
      (void)req;
        res.set_content("<html>hello</html>","123");
      });

  ser.set_base_dir("./wwwroot");

  ser.listen("0.0.0.0",9090);

  return 0;
}
#endif

















