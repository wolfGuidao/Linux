#if defined(GNUC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored “-Wdeprecated-declarations”
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif
#include "db.hpp"

void TestDishTable()
{
  MYSQL* mysql = order_system::MYSQLInit();
  order_system::DishTable dish_table(mysql);

  //1.插入数据
#if 0
  Json::Value dish;
  dish["name"]="宫保鸡丁";
  dish["price"]=1000;

  bool ret = dish_table.Insert(dish);
  printf("%d\n",ret);
#endif
  
  //2.查找所有
  Json::Value dishes;
  int ret = dish_table.SelectAll(&dishes);
  printf("%d\n",ret);
  Json::StyledWriter writer;
  printf("%s\n",writer.write(dishes).c_str());

  order_system::MYSQLRelease(mysql);
}

void TestOrderTable()
{

}

int main()
{
  TestDishTable();
  //TestOrderTable();
  return 0;
}
