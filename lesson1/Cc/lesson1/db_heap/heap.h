#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<algorithm>
using namespace std;

typedef int HPDataType;
typedef struct Heap
{
    HPDataType* _a;
      int _size;
        int _capacity;

}Heap;

void PPrint(int * arr, int len);

void Down(int* arr, int len, int parent);

void Up(int * arr, int child);

void HeapUp(Heap* hp, int index);

void HeapDown(Heap* hp, int index);

void Print(Heap * hp);

// 堆的构建
 void HeapCreate(Heap* hp, HPDataType* a, int n);
 // 堆的销毁
 void HeapDestory(Heap* hp);
 // 堆的插入
 void HeapPush(Heap* hp, HPDataType x);
 // 堆的删除
 void HeapPop(Heap* hp);
 // 取堆顶的数据
 HPDataType HeapTop(Heap* hp);
 // 堆的数据个数
 int HeapSize(Heap* hp);
 // 堆的判空
 int HeapEmpty(Heap* hp);

 // 对数组进行堆排序
 void HeapSort(int* a, int n);
 
