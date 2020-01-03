#include"heap.h"

// 堆的构建
void HeapCreate(Heap* hp, HPDataType* a, int n)
{
  hp->_a = (HPDataType*)malloc(sizeof(HPDataType)*n);
  memcpy(hp->_a, a, sizeof(HPDataType)*n);
  hp->_size = n;
  hp->_capacity = n;

  for (int i = (n - 1 - 1) / 2; i >= 0; i--)
  {
    HeapDown(hp, i);
  }

}
// 堆的销毁
void HeapDestory(Heap* hp)
{
  free(hp->_a);
  hp->_a = nullptr;
}
// 堆的插入
void HeapPush(Heap* hp, HPDataType x)
{
  if (hp->_size == hp->_capacity)
  {
    hp->_a = (HPDataType*)realloc(hp->_a ,hp->_capacity * 2 * sizeof(HPDataType));
    hp->_capacity *= 2;
  }

  hp->_a[hp->_size] = x;
  hp->_size++;

  HeapUp(hp, hp->_size - 1);
}

// 堆的删除
void HeapPop(Heap* hp)
{
  if(hp->_a == nullptr)
  {
    return ;
  }

  swap(hp->_a [0],hp->_a [hp->_size -1]);
  hp->_size --;
  HeapDown(hp,0);
}

// 取堆顶的数据
HPDataType HeapTop(Heap* hp)
{
  if(hp->_a ==nullptr)
  {
    return -1;
  }
  return hp->_a [0];
}
// 堆的数据个数
int HeapSize(Heap* hp)
{
  return hp->_size ;
}
// 堆的判空
int HeapEmpty(Heap* hp)
{
  if(hp->_size ==0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

// 对数组进行堆排序
void HeapSort(int* a, int n)
{
  for (int i = (n - 2) / 2; i >= 0; i--)
  {
    Down(a, n, i);
  }
  /*
     for (int i = 0; i < n ; i++)
     {
     swap(a[0], a[n - 1 - i]);

     Down(a, n - i, 0);
     }
     */
  int end = n - 1;
  while (end)
  {
    swap(a[0], a[end]);
    Down(a, end, 0);
    end--;
  }

}

void Up(int * arr, int child)
{
  int parent = (child - 1) / 2;
  while (child > 0)
  {
    if(child>0&& arr[parent]>arr[child])
    {
      swap(arr[child],arr[parent]);
      child = parent;
      parent = (child - 1) / 2;
    }
    else
    {
      break;
    }
  }
}

void PPrint(int * a,int n)
{
  for (int i = 0; i < n; i++)
  {
    cout << a[i] << " ";

  }
  cout << endl;
}
void HeapDown(Heap* hp, int index)
{
  HPDataType child = index * 2 + 1;
  while (child < hp->_size)
  {
    if ((index * 2 + 2) < hp->_size && hp->_a[child] > hp->_a[child + 1])
    {
      child = child + 1;
    }

    if (hp->_a[index] > hp->_a[child])
    {
      swap(hp->_a[index], hp->_a[child]);
      index = child;
      child = index * 2 + 1;
    }
    else
    {
      break;
    }
  }
}

void HeapUp(Heap* hp, int index)
{
  HPDataType parent = (index - 1) / 2;
  while (index > 0)
  {
    if (hp->_a[index] < hp->_a[parent])
    {
      swap(hp->_a[index], hp->_a[parent]);
      index = parent;
      parent = (index - 1) / 2;
    }
    else
    {
      break;
    }
  }
}
void Print(Heap * hp)
{
  for(int i = 0; i < hp->_size; i++)
  {
    cout << hp->_a[i] << " ";
  }
  cout << endl;
}

void Down(HPDataType* arr, int len ,int parent)
{
   int child = parent * 2 + 1;
  while (child<len)
  {
    if (child + 1 < len && arr[child] > arr[child + 1])
    {
      //swap(arr[child], arr[child + 1]);
      child = child + 1;
    }
    if(arr[parent]>arr[child])
    {
      swap(arr[parent], arr[child]);
      parent = child;
      child = parent * 2 + 1;
    }
    else
    {
      break;
    }
  }
}

