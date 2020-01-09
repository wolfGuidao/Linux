#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

//交换函数
void Swap(int* a,int* b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

//打印函数
void Print(int*arr,int len)
{
  for(int i = 0;i < len;i++)
  {
    printf("%d ",arr[i]);
  }
  printf("\n");
}

//选择排序
//定义两个下标，分别记录每一趟最大值最小值的位置，然后交换，把最小值放在begin位置，最大值放在end位置处
void SelectSort(int* arr,int len)
{
  int begin = 0;
  int end = len - 1;
  while(begin < end)
  {
    int min_index = begin;
    int max_index = end;
    for(int i = begin;i <= end;i++)
    {
      if(arr[i] > arr[max_index])
      {
        max_index = i;
      }
      if(arr[i] < arr[min_index])
      {
        min_index = i;
      }
    }

    Swap(&arr[begin],&arr[min_index]);

    //交换的时候需要注意，如果最大值刚好在begin位置处，经过前面的交换，最大值已经杯换到最小下标处
    if(max_index == begin)
    {
      max_index = min_index; 
    }

    Swap(&arr[end],&arr[max_index]); 
    begin++;
    end--;
  }
}

//插入排序
//用end标记有序段的最后一个元素位置，注意临界条件
void InsertSort(int* arr,int len)
{
  int end = 0;

  for(end =0 ;end < len -1;end ++)
  {
    int temp = arr[end +1];
    while(end >= 0)
    {
      if(arr[end] > temp)
      {
        arr[end + 1] = arr[end];
        end--;
      }
      else
      {
        break;
      }
    }
    arr[end + 1] = temp; 
  }
}

//希尔排序
//在插入排序的基础上每次进行gap个跳跃
void ShellSort(int* arr,int len)
{
  int gap = len;
  while(gap > 1)
  {
    gap = gap / 3 +1;
    for(int i = 0;i < len - gap;i++)
    {
      int end = i;
      int temp = arr[end + gap];
      while(end >= 0)
      {
        if(arr[end] > temp)
        {
          arr[end + gap] = arr[end];
          end -= gap;
        }
        else
        {
          break;
        }
      }
      arr[end + gap] = temp;
    }
  }
}


//向下调整
void adjust(int* arr,int len,int index)
{
  int parent = index;
  int child = parent * 2 + 1;
  while(child < len)
  {
    if(child + 1 < len && arr[child] < arr[child + 1])
    {
      child = child + 1;
    }
    if(arr[child] > arr[parent])
    {
      Swap(&arr[child],&arr[parent]);
      parent = child;
      child = parent * 2 + 1;
    }
    else
    {
      break;
    }
  }
}

//堆排
void HeapSort(int* arr,int len)
{
  for(int i = (len - 1 - 1 ) / 2;i >= 0;i--)
  {
    adjust(arr, len, i);
  }

  int end = len - 1;

  while(end > 0)
  {
    Swap(&arr[0],&arr[end]);
    adjust(arr,end,0);
    end--;
  }
}

//一趟快排
int QuickSortChild1(int* arr,int begin,int end)
{
  int key = begin;
  while(begin < end)
  {
    while(begin < end && arr[end] >= arr[key])
    {
      end--;
    }
    while(begin < end && arr[begin] <= arr[key])
    {
      begin++;
    }

    Swap(&arr[end],&arr[begin]);
  }
  Swap(&arr[key],&arr[begin]); 
  return begin;
}

//快排
void QuickSort(int* arr,int begin,int end)
{
  //注意递归出口
  if(begin < end)
  {
    int mid = QuickSortChild1(arr,begin,end);
    QuickSort(arr,begin,mid - 1);
    QuickSort(arr,mid + 1,end);

  }
}

//快排优化--->三数取中,防止有序的情况
void medianOfThree(int* arr,int begin,int end)
{
  int mid = (begin + end) >> 1;
  if(arr[mid] > arr[begin])
  {
    Swap(&arr[mid],&arr[begin]);
  }
  if(arr[mid] > arr[end])
  {
    Swap(&arr[mid],&arr[end]);
  }
  if(arr[end] < arr[begin])
  {
    Swap(&arr[end],&arr[begin]);
  }
}

void QuickSort1(int* arr,int begin,int end)
{
  if(begin < end)
  {
    medianOfThree(arr,begin,end);
    int mid = QuickSortChild1(arr,begin,end);
    QuickSort1(arr,begin,mid - 1);
    QuickSort1(arr,mid + 1,end);
  }
}

//挖坑法
int QuickSortChild2(int* arr,int begin,int end)
{
  int key =arr[begin];
  while(begin < end)
  {
    while(begin < end && arr[end] >= key)
    {
      end--;
    }
    arr[begin] = arr[end];

    while(begin < end && arr[begin] <= key)
    {
      begin++;
    }
    arr[end] = arr[begin];
  }
  arr[begin] = key;
  return begin;
}

//双指针法
int QuickSortChild3(int* arr,int begin,int end)
{
  int pref = begin - 1;
  int cur = begin;
  int key = end;
  //定义pref，cur 。pref指向cur的前一个
  while(cur < end)
  {
    //cur去找比key小的元素，如果找到，就把pref++，就交换cur和pref下标所在的元素
    if(arr[cur] < arr[key])
    {
      pref++;
      Swap(&arr[cur],&arr[pref]);
    }
    //如果cur所在下标的元素比key所在元素大，就直接后移cur
    cur++;
  }
  //最后注意和end位置的元素交换的是pref的下一个,不是它自己
  Swap(&arr[++pref],&arr[end]);
  return pref;
}

//快排优化--->小区间优化 + 三数取中
void QuickSort2(int* arr,int begin,int end)
{
  if(begin < end)
  {
    if(end - begin + 1 < 10)
    {
      SelectSort(arr + begin,end - begin + 1);
    }
    else
    {

      medianOfThree(arr,begin,end);
      int mid = QuickSortChild3(arr,begin,end);
      QuickSort1(arr,begin,mid - 1);
      QuickSort1(arr,mid + 1,end);

    }
  }
}

//测试
void TestSelectSort()
{
  int arr[] ={2,5,4,9,8,7,6,3,1,0};
  SelectSort(arr,sizeof(arr)/sizeof(int));
  Print(arr,sizeof(arr)/sizeof(int));
}

void TestInsertSort()
{
  int arr[] ={2,5,4,9,8,7,6,3,1,0};
  InsertSort(arr,sizeof(arr)/sizeof(int));
  Print(arr,sizeof(arr)/sizeof(int));

}

void TestShellSort()
{
  int arr[] ={2,5,4,9,8,7,6,3,1,0};
  ShellSort(arr,sizeof(arr)/sizeof(int));
  Print(arr,sizeof(arr)/sizeof(int));
}

void TestHeapSort()
{
  int arr[] ={2,5,4,9,8,7,6,3,1,0};
  HeapSort(arr,sizeof(arr)/sizeof(int));
  Print(arr,sizeof(arr)/sizeof(int));
}

void TestQuickSort()
{
  int arr[] ={2,5,4,9,8,7,6,3,1,0};
  QuickSort(arr,0,sizeof(arr)/sizeof(int) -1 );
  Print(arr,sizeof(arr)/sizeof(int));
}

void TestQuickSort1()
{
  int arr[] ={2,5,4,9,8,7,6,3,1,0};
  QuickSort1(arr,0,sizeof(arr)/sizeof(int) -1 );
  Print(arr,sizeof(arr)/sizeof(int));
}

void TestQuickSort2()
{
  int arr[] ={2,5,4,9,8,7,6,3,1,0};
  QuickSort2(arr,0,sizeof(arr)/sizeof(int) -1 );
  Print(arr,sizeof(arr)/sizeof(int));
}

//测试每个排序的时间
void TestOP()
{
  srand(time(0));
  const int N = 10000;
  int* a1 = (int*)malloc(sizeof(int)*N);
  int* a2 = (int*)malloc(sizeof(int)*N);
  int* a3 = (int*)malloc(sizeof(int)*N);
  int* a4 = (int*)malloc(sizeof(int)*N);
  int* a5 = (int*)malloc(sizeof(int)*N);
  int* a6 = (int*)malloc(sizeof(int)*N);
  for (int i = 0; i < N; ++i)
  {
    a1[i] = rand();
    a2[i] = a1[i];
    a3[i] = a1[i];
    a4[i] = a1[i];
    a5[i] = a1[i];
    a6[i] = a1[i];

  }
  int begin1 = clock();
  InsertSort(a1, N);
  int end1 = clock();
  int begin2 = clock();
  ShellSort(a2, N);
  int end2 = clock();
  int begin3 = clock();
  SelectSort(a3, N);
  int end3 = clock();
  int begin4 = clock();
  HeapSort(a4, N);
  int end4 = clock();
  int begin5 = clock();
  QuickSort(a5, 0, N-1);
  int end5 = clock();
  int begin6 = clock();
  //MergeSort(a6, N);
  int end6 = clock();
  printf("InsertSort:%d\n", end1 - begin1);
  printf("ShellSort:%d\n", end2 - begin2);
  printf("SelectSort:%d\n", end3 - begin3);
  printf("HeapSort:%d\n", end4 - begin4);
  printf("QuickSort:%d\n", end5 - begin5);
  printf("MergeSort:%d\n", end6 - begin6);
  free(a1);
  free(a2);
  free(a3);
  free(a4);
  free(a5);
  free(a6);

}

int main()
{
  //TestSelectSort();
  //TestInsertSort();
  //TestShellSort();
  //TestHeapSort();
  //TestQuickSort();
  //TestQuickSort1();
  //TestOP();
  TestQuickSort2();
  return 0;
}
