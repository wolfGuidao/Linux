#include"udp_server.hpp"
#include<unordered_map>
//能放到vector,list,deque,array中的对象：1）、能拷贝的对象 2）、支持移动语义
//能放到set中的对象：1）、能拷贝  2）、能够比较
//能放到map中的对象：对于key ：1）、能拷贝  2）、能够比较 对于value：能拷贝
//能放到unordered_map中的对象：对于value：能拷贝 对于key：1）、能拷贝 2）、能比较相等 3）、能哈希


//创建一个中英文翻译的服务器
int main()
{
    UdpServer server;
    server.Start("0.0.0.0",9090,[](const string& req,string* resp)
            {
                unordered_map<string,string>dict;
                dict.insert(make_pair("hello","你好"));
                dict.insert(make_pair("world","世界"));
                dict.insert(make_pair("bite","比特"));
                auto it = dict.find(req);
                if(it==dict.end())
                {
                *resp="未找到结果";
                }
                else
                {
                    *resp=it->second;
                    printf("resp:%s\n",resp->c_str());
                }
            });
    return 0;
}
