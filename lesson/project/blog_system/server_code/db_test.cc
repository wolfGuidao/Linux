#include"db.hpp"

//该程序用来测试封装的MySQL操作

void TestBlogTest()
{
    Json::StyledWriter writer;//方便序列化进行打印输出
    MYSQL* mysql=blog_system::MySQLInit();
    blog_system::BlogTable blog_table(mysql);
    bool ret=false;

    //测试插入
    Json::Value blog;
    blog["title"]="第一个项目";
    blog["content"]="我要找工作";
    blog["tag_id"]=1;
    blog["create_time"]="2019/12/20";

    ret = blog_table.Insert(blog);
    printf("insert:%d\n",ret);

    //测试查找
    Json::Value blogs;
    ret = blog_table.SelectAll(&blogs);
    printf("select All %d\n",ret);
    //序列化程字符串，调用writer输出
    printf("blog:%s\n",writer.write(blogs).c_str());

    //测试查找单个blog
    ret = blog_table.SelectOne(1,&blog);
    printf("selecr one %d\n",ret);
    printf("blog:%s\n",writer.write(blog).c_str());

    //测试修改博客
    blog["blog_id"]=1;
    blog["title"]="要你管";
    blog["content"]="1.我'喜欢你'；2.你''喜欢'我\n 3.你自己选";
    ret=blog_table.Update(blog);
    printf("update:%d\n",ret);
    printf("update:%s\n",writer.write(blog).c_str());

    //测试删除博客
    ret=blog_table.Delete(3);
    printf("delete:%d\n",ret);

    blog_system::MySQLRelease(mysql);
}

void TestTagTable()
{
    MYSQL* mysql = blog_system::MySQLInit();
    blog_system::TagTable tag_table(mysql);
    Json::Value tag;
    bool ret=false;

    //测试插入
    tag["tag_name"]="要你管";
    /*
    ret = tag_table.Insert(tag);
    printf("tag Insert:%d\n",ret);
    */

    //测试查找 
    Json::Value tags;
    ret = tag_table.SelectAll(&tags);
    Json::StyledWriter writer ;
    printf("select All %d\n",ret);
    //序列化程字符串，调用writer输出
    printf("blog:%s\n",writer.write(tags).c_str());

    //测试删除
    ret = tag_table.Delete(1);
    printf("Delete: %d\n",ret);

    blog_system::MySQLRelease(mysql);
}

int main()
{
    //TestBlogTest();
    TestTagTable();
    return 0;
}
