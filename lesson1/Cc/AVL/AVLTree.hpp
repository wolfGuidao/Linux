#pragma once
#include <iostream>
using namespace std;

template<class T>
struct AVLTreeNode
{
  //T如果是内置类型，那么T()就是0，如果T是自定义类型，T()相当于Date(),即该Date类必须提供无参的构造函数
  AVLTreeNode(const T& data = T())
    :_pLeft(nullptr)
     ,_pRight(nullptr)
     ,_data(data)
     ,_bf(0)
     ,_pParent(nullptr)
  {

  }
  AVLTreeNode<T>* _pLeft;
  AVLTreeNode<T>* _pRight;
  AVLTreeNode<T>* _pParent;
  T _data;
  int _bf;//记录平衡因子
};

template<class T>
class AVLTree
{
  typedef AVLTreeNode<T>Node;

  public:
  AVLTree()
    :_pRoot(nullptr)
  {}

  bool Insert(const T& data)
  {
    //AVL树可能是空树
    if(nullptr == _pRoot)
    {
      _pRoot = new Node(data);
      return true;
    }

    //AVL树非空
    //先按照二叉搜索树的方式插入新节点
    Node* pCur = _pRoot;
    Node* pParent = nullptr;

    //循环找到插入pCur节点的位置
    while(pCur)
    {
      //保存双亲节点，方便插入
      pParent = pCur;

      if(data < pCur->_data)
      {
        //如果插入数据比当前节点的值小，说明应该查到当前节点的左子树中
        pCur = pCur->_pLeft;
      }
      else if(data > pCur->_data)
      {
        //如果插入数据比当前节点的值大，说明应该查到当前节点的右子树中
        pCur = pCur->_pRight;
      }
      else
      {
        //如果插入数据比当前节点的值相等，说明当前数据已存在，不用插入
        return false;
      }
    }

    //插入新节点
    pCur = new Node(data);
    if(data < pParent->_data)
    {
      pParent->_pLeft = pCur;
    }
    else
    {
      pParent->_pRight = pCur;
    }

    //注意节点中加了_pParent指针所以插入节点后需要更新
    pCur->_pParent = pParent;

    //更新平衡因子
    //如果插入节点之前：pParent的平衡因子情况
    //0--->pParent是叶子节点---->pCur可能插在pParent的左侧|右侧--->插入pCur后对整棵树有影，需要向上调整
    //1--->pParent只有右孩子--->pCur只能插在pParent的左侧--->没影响，不用调整---->插入后pParent的平衡因子为0
    //-1--->pParent只有左孩子--->pCur只能插在pParent的右侧--->没影响，不用调整--->插入后pParent的平衡因子为0
    while(pParent)
    {
      //更新pParent节点的平衡因子,这里规定 平衡因子 = 右子树高度 - 左子树高度
      if(pCur == pParent->_pLeft)
      {
        //如果节点插在pParent的左子树上，就直接把平衡因子--即可
        pParent->_bf--;
      }
      else
      {
        //如果节点插在pParent的左子树上，就直接把平衡因子++即可
        pParent->_bf++;
      }

      if(0 == pParent->_bf)
      {
        //如果插入完成后，pParent节点的平衡因子等于0，那么插入新节点后对整棵树是没有任何影响的，所以直接返回
        return true;
      }
      else if(-1 == pParent->_bf || 1 == pParent->_bf)
      {
        //代表插入新节点后对整棵树是有影响的，所以需要向上进行更新整棵树的平衡因子
        pCur = pParent;
        pParent = pCur->_pParent;
      } 
      else
      {
        //pParent的平衡因子为+2/-2
        //说明pParent节点已经违反AVL树的性质,需要调整,即要对 以pParent为根节点 进行旋转调整
        //左单旋：新节点插在较高右子树的右侧
        //右单旋：新节点插在较高左子树的左侧
        //左右双旋：
        //右左双旋：
        if(pParent->_bf == 2)
        {
          //如果parent的平衡因子==2，说明右子树较高
          if(pCur->_bf == 1)
          {
            //单旋
            RotateL(pParent);
          }
          else
          {
            //-1,
            RotateRL(pParent);
          }
        }
        else//-2
        {
          //如果parent的平衡因子==-2，说明左子树较高
          if(pCur->_bf == -1)
          {
            RotateR(pParent);
          }
          else
          {
            RotateLR(pParent);
          }
        }
        break;//新节点插入经过旋转后，这棵树的高度并没有增加，所以就不用向上继续调整了
      }
    }
    return true;
  }

  void Inorder()
  {
    _Inorder(_pRoot);
  }

  bool IsValidAVLTree()
  {
    return _IsValidAVLTree(_pRoot);
  }

  private:

  //右单旋
  void RotateR(Node* pParent)
  {
    Node* pSubL = pParent->_pLeft;
    Node* pSubLR = pSubL->_pRight;

    //链接，改变左右孩子指针指向,双亲
    pParent->_pLeft = pSubLR;//1
    if(pSubLR)
    {
      pSubLR->_pParent = pParent;//更新双亲
    }

    pSubL->_pRight = pParent;//2

    //用来保存pParent的双亲
    Node* pPParent = pParent->_pParent;

    pSubL->_pParent = pPParent;//更新双亲
    pParent->_pParent = pSubL;//更新双亲

    if(pPParent == nullptr)//更新双亲
    {
      //说明子树原来pParent为根节点
      _pRoot = pSubL;
    }
    else
    {
      //说明子树原来pParent不是根节点，只是某个子树，那么就只需分两种情况
      if(pPParent->_pLeft == pParent)
      {
        pPParent->_pLeft = pSubL;
      }
      else
      {
        pPParent->_pRight = pSubL;
      }
    }

    //更新平衡因子
    pParent->_bf = pSubL->_bf = 0;
  }

  //左单旋
  void RotateL(Node* pParent)
  {
    Node* pSubR = pParent->_pRight;
    Node* pSubRL = pSubR->_pLeft;

    pParent->_pRight = pSubRL;
    if(pSubRL)
    {
      pSubRL->_pParent = pParent;
    }

    pSubR->_pLeft = pParent;
    Node* pPParent = pParent->_pParent;
    pSubR->_pParent = pPParent;
    pParent->_pParent = pSubR;

    if(pPParent == nullptr)
    {
      _pRoot = pSubR;
    }
    else
    {
      if(pPParent->_pLeft == pParent)
      {
        pPParent->_pLeft = pSubR;
      }
      else
      {
        pPParent->_pRight = pSubR;
      }
    }

    pParent->_bf = pSubR->_bf = 0;
  }

  //右左双旋
  void RotateRL(Node* pParent)
  {
    //保存调整前的平衡因子，方便更新
    Node* pSubR = pParent->_pRight;
    Node* pSubRL = pSubR->_pLeft;
    int bf = pSubRL->_bf;

    RotateR(pParent->_pRight);
    RotateL(pParent);

    //需要更新部分节点的平衡因子
    if(bf == 1)
    {
      pParent->_bf = -1;
    }
    else if(bf == -1)
    {
      pSubR->_bf = 1;
    }

  }

  //左右双旋
  void RotateLR(Node* pParent)
  {
    //保存调整前的平衡因子，方便更新
    Node* pSubL = pParent->_pLeft;
    Node* pSubRL = pSubL->_pRight;
    int bf = pSubRL->_bf;

    RotateL(pParent->_pLeft);
    RotateR(pParent);

    if(bf == 1)
    {
      pSubL->_bf = -1;
    }
    else if(bf == -1)
    {
      pParent->_bf = 1;
    }
  }

  //遍历
  void _Inorder(Node* pRoot)
  {
    if(pRoot)
    {
      _Inorder(pRoot->_pLeft);
      cout<<pRoot->_data<<" ";
      _Inorder(pRoot->_pRight);
    }
  }

  //检测
  bool _IsValidAVLTree(Node* pRoot)
  {
    if(nullptr == pRoot)
    {
      return true;
    }
    int leftHight = Hight(pRoot->_pLeft);
    int rightHight = Hight(pRoot->_pRight);
    int bf = rightHight - leftHight;

    if(abs(bf) <= 1 && bf == pRoot->_bf)
    {
      return _IsValidAVLTree(pRoot->_pLeft)&&
      _IsValidAVLTree(pRoot->_pRight);
    }
    {
      return false;
    }
  }

  int Hight(Node* pRoot)
  {
    if(pRoot == nullptr)
    {
      return 0;
    }
    
    int leftHight = Hight(pRoot->_pLeft);
    int rightHight = Hight(pRoot->_pRight);

    return leftHight > rightHight ? leftHight+1 : rightHight+1;
  }

  private:

  Node* _pRoot;
};

