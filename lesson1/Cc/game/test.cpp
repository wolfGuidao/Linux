#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//////宏定义//////////////////////////////
#define MAPWIDTH 60
#define MAPHEIGHT 30
#define SNAKESIZE 100
///////数据设计/////////////////////////////
struct
{
  int x;
  int y;    //食物坐标
}food;

struct
{
  int x[SNAKESIZE];    //蛇的节数和坐标
  int y[SNAKESIZE];
  int length;         //蛇的长度
  int speed;          //蛇的速度
}snake;
/////////////全局变量//////////////////////
int changeflag = 0; //蛇的增长标志.
char  key = 's'; //初始化蛇向下运动

/////////////////模块化设计/////////////////
//1. 画地图
void DrawMap();
//2. 产生食物
void CreateFood();
//3. 按键操作
void KeyDown();
//4. 蛇的状态:判断是否结束游戏
int SnakeStatus();
//5. 辅助函数: 光标移动
void gotoxy(int x, int y);

int main(void)
{
  DrawMap();
  while (1)
  {
    CreateFood();
    gotoxy(MAPWIDTH + 2, MAPHEIGHT / 2);
    printf("score: %d", snake.length - 3);
    Sleep(snake.speed); //sleep函数,执行挂起一段时间
    KeyDown();
    if (!SnakeStatus())
      break;
  }
  gotoxy(MAPWIDTH / 2 - 10, MAPHEIGHT / 2);
  printf("Game Over!");
  system("pause");
  return 0;
}

////////////////////////////////1. 画地图/////////////////////////////////////////////
// #: 食物 @: 蛇头, 边框  *:蛇身
void DrawMap()
{
  //游戏声明
  gotoxy(MAPWIDTH + 2, MAPHEIGHT / 2 - 3);
  printf("Tab 'a','d','s','w' to move.");
  srand((unsigned int)time(NULL)); //根据时间变化的种子

  //1. 圈地
  //1.1打印竖边框
  for (int i = 0; i <= MAPHEIGHT; i++)
  {
    gotoxy(0, i); //物理坐标系
    printf("█");
    gotoxy(MAPWIDTH, i);
    printf("█");
  }
  //1.2打印横边框
  for (int i = 0; i <= MAPWIDTH; i += 2) // █占用两个字符.
  {
    gotoxy(i, 0);
    printf("█");
    gotoxy(i, MAPHEIGHT);
    printf("█");
  }
  //2. 画蛇
  //2.1确定蛇的长度,速度
  snake.length = 3;
  snake.speed = 300;
  //2.2确定蛇头的坐标(地图中央)
  snake.x[0] = MAPWIDTH / 2;
  snake.y[0] = MAPHEIGHT / 2;
  //2.3画蛇头
  gotoxy(snake.x[0], snake.y[0]);
  printf("@");
  //2.3画剩下身体
  for (int i = 1; i < snake.length; i++)
  {
    snake.x[i] = snake.x[i - 1] + 1;
    snake.y[i] = snake.y[i - 1];
    gotoxy(snake.x[i], snake.y[i]);
    printf("*");
  }
  //3. 画食物
  //3.1 产生食物坐标
  food.x = rand() % (MAPWIDTH - 4) + 2; //随机坐标.[2-57] 
  food.y = rand() % (MAPHEIGHT - 2) + 1; //随机范围在地图长和宽内, 且平移至地图内[1 28]
  //3.2 打印食物
  gotoxy(food.x, food.y);
  printf("#");
}
//2. 产生食物
void CreateFood()
{
  //蛇吃食物----蛇头和食物坐标重合
  if (snake.x[0] == food.x && snake.y[0] == food.y)
  {
    //产生新食物满足条件：(1)食物不在蛇身上, (2)横坐标为偶数(因为蛇身一格占两字符)
    srand((unsigned int)time(NULL));
    while (1)
    {
      int flag = 1;
      food.x = rand() % (MAPWIDTH - 4) + 2; //[2 - 57]
      food.y = rand() % (MAPHEIGHT - 2) + 1; //[1 28]

      for (int k = 0; k < snake.length; k++)
      {
        if (snake.x[k] == food.x && snake.y[k] == food.y) {
          flag = 0;
          break;               //flag--判断食物位置是否正确的标志
        }
      }
      if (flag== 1) {
        break;
      }
    }
    gotoxy(food.x, food.y);
    printf("#");
    snake.length++;
    snake.speed -= 10;
    changeflag = 1;
  }
  //产生新食物
}
//////////////////////////3. 按键操作///////////////////////////////////////
void KeyDown()
{
  char temp;
  //1.接收按键
  if (_kbhit()) //keyboard hit 检查是否有按键;
  {
    fflush(stdin); //清除缓存区
    key = _getch();
  }
  //无按键处理,蛇自由移动
  /*
   *蛇移动时候先擦去蛇尾的一节
   *changeFlag为0表明此时没有吃到食物，因此每走一步就要擦除掉蛇尾，以此营造一个移动的效果
   *为1表明吃到了食物，就不需要擦除蛇尾，以此营造一个蛇身增长的效果
   */
  if (!changeflag)
  {
    //擦除最后一节蛇身
    gotoxy(snake.x[snake.length - 1], snake.y[snake.length - 1]);
    printf(" ");
  }
  //后面蛇身的移动
  for (int i = snake.length - 1; i > 0; i--)
  {
    snake.x[i] = snake.x[i - 1];
    snake.y[i] = snake.y[i - 1];
    //将蛇身变为'*'
    gotoxy(snake.x[i], snake.y[i]);
    printf("*");
  }

  //2.有按键
  switch (key)
  {
    case 'w':
      snake.y[0]--;
      break;
    case 's':
      snake.y[0]++;
      break;
    case 'a':
      snake.x[0] = snake.x[0] - 1; //一个@占1个字符
      break;
    case 'd':
      snake.x[0] = snake.x[0] + 1;
      break;
    default:
      break;
  }
  gotoxy(snake.x[0], snake.y[0]);
  printf("@");   //打印出蛇头(前进)
  changeflag = 0;
  //打印完蛇身后将光标移到屏幕最上方，避免光标在蛇身处一直闪烁
  gotoxy(MAPWIDTH + 2, 0);  

}
//4. 蛇的状态:判断是否结束游戏
int SnakeStatus()
{
  //撞墙判断
  if (snake.x[0] == 0 || snake.x[0] == MAPWIDTH - 1 || snake.y[0] == 0 || snake.y[0] == MAPHEIGHT)//?????????????????
    return 0;
  //判断撞自己
  for (int i = 1; i < snake.length; i++)
  {
    if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i])
      return 0;
  }
  return 1;   //记得返回1.    
}
//5. 辅助函数: 光标移动
void gotoxy(int x, int y)
{
  //调用API去设置控制台的光标位置(以打印东西)
  //1. 找到控制台的窗口
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  //2. 光标结构体
  COORD coord;
  //3. 设置光标的坐标
  coord.X = x;
  coord.Y = y;
  //4. 同步到控制台 set console cursor position
  SetConsoleCursorPosition(handle, coord);
}
