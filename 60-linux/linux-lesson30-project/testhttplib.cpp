#include <stdio.h>
#include "httplib.h"

using namespace httplib;

void func(const Request& req, Response& resp)
{
    //req : http 请求的内容
    //resp ：需要给用响应的内容
    resp.set_content("<html>hello bite</html>", "123");
}

int main()
{
    Server svr;
    
    svr.Get("/", [](const Request& req, Response& resp){
            (void)req;
            resp.set_content("<html>linux-60</html>", "123");
            });
    svr.listen("172.16.99.129", 19999);
    return 0;
}
