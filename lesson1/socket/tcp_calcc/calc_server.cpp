#include<sstream>
#include<jsoncpp/json/json.h>
#include"tcp_thread_server.hpp"

//使用Json的方式进行序列化和反序列化
//{
//“nms1"[10,20,30];
//"op":"+"
//}
//{
//"result":30;
//}
//解析Json格式:
//使用jsoncpp库,Json::Value-->用来表示一个Json数据
//Json::Writer表示把json::value转成字符串
//Json::Reader表示把字符串转成Json::value
int main()
{
    TcpThreadServer server;
    server.Start("0.0.0.0",9090,[](const string& req, string*resp){
            //1.将req反序列化
            Json::Reader reader;
            Json::Value req_json;//输出型参数
            reader.parse(req,req_json);

            //2.根据反序列化的结果，进行计算
            //Json::Value& nums = req_json["nums"];
            //string op=req_json["op"].asString();

            //int result=0;
//            for(size_t i=0;i<nums.size();i++)
//            {
//                if( op== "+")
//                {
//                    result += nums[(int)i].asInt();
//                }
//                else if(op=="-")
//                {
//                    result -= nums[(int)i].asInt();
//                }
//            }
//
            int num1=req_json["num1"].asInt();
            int num2=req_json["num2"].asInt();
            string op=req_json["op"].asString();
            int result =0;
            if(op=="+")
            {
                result=num1+num2;
            }
            else if(op=="-")
            {
                result = num1-num2;
            }

            //3.把结果序列化为json字符串
            Json::Value resp_json;
            Json::FastWriter writer;
            resp_json["result:"]=result;
            *resp = writer.write(resp_json);
            printf("result:%d\n",result);

            });
    return 0;
}
