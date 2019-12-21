#include<sstream>
#include<jsoncpp/json/json.h>
#include"tcp_client.hpp"

int main(int argc,char* argv[])
{
    //1.创建一个tcp客户端
    TcpClient client;
    bool ret = client.Connect(argv[1],9090);
    if(!ret)
    {
        printf("Connect");
        return 1;
    }
    //2.循环读入客户端输入的数据
    while(1)
    {
        printf("请输入两个操作数num1:num2及操作符：\n");
        fflush(stdout);
        int num1=0,num2=0;
        char op[10]={0};
        scanf("%d%d%s",&num1,&num2,op);
        printf("请输入操作符op:\n");

        //3.根据用户输入的内容，构造一个Json对象
        Json::Value req_json;
        req_json["num1"]=num1;
        req_json["num2"]=num2;
        req_json["op"]=op;

        //4.把Json对象序列化成一个字符串
        Json::StyledWriter writer;
        string req=writer.write(req_json);
        printf("req:%s\n",req.c_str());

        //5.把字符串发送到服务器端
        client.Send(req);

        //6.从服务器读取返回结果
        string resp;
        client.Recv(&resp);
        printf("%s",resp.c_str());
        
        //7.把服务器读取结果进行Json解析
        Json::Reader reader;
        Json::Value resp_json;
        reader.parse(resp,resp_json);
        printf("result:%d\n",resp_json["result"].asInt());
        
    }
    return 0;
}
