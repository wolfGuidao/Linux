#include"httplib.h"


void Handler(const httplib::Request& req,httplib::Response& resp)
{
    (void)req;
    resp.set_content("<html><h1>hello world!</h1></html>","text/html");
}


int main()
{
    using namespace httplib;
    Server server;
    //处理GET方法的请求
    server.Get("/",Handler);
    server.set_base_dir("./wwwroot");
    server.listen("0.0.0.0",9093);
    return 0;
}
