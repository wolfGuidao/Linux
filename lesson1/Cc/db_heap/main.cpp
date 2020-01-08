#include"heap.h"

void test()
{
  Heap hp;
  int arr[10] = { 2,8,7,6,5,4,3,2,1,0  };
  HeapCreate(&hp, arr, 10);
  Print(&hp);
  HeapPush(&hp,999); 
  Print(&hp);
  HeapPop(&hp);
  Print(&hp);
  cout<<HeapEmpty(&hp)<<endl;
  cout<<HeapTop(&hp)<<endl;
  //HeapSort(arr, 10);
  //PPrint(arr,10);

}


int main()
{

  test();
  system("pause");
  return 0;

}


/*
#include<iostream>
#include<vector>
#include<string>
#include<stack>
using namespace std;

int main()
{
#if 1
int n;
//while (cin >> n)
cin >> n;
{
vector<string>ret;
string str;
stack<int>s;
n = n + 1;
while (n)
{
getline(cin,str);
ret.push_back(str);
n--;
}

for (int i = 0; i < ret.size(); i++)
{
if (ret[i][0] == 'P')
{
s.push(ret[i][1]);
}
if (ret[i][0] == 'O' && !s.empty())
{
s.pop();
}
if (ret[i][0] == 'A')
{
if (s.empty())
{
printf("E\n");
}
else
{
printf("%d\n", s.top());
}
}
}
}
#endif
#if 0
string str;
vector<string>ret;
int i = 5;
while (i)
{
getline(cin, str);
i--;
}
for (int i = 0; i < ret.size(); i++)
{
cout << ret[i] << endl;
}
#endif
system("pause");
return 0;


}
*/
