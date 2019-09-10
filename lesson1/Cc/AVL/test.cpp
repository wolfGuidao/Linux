#include "AVLTree.hpp"

void TestAVLTree()
{
  int array[] = {16,3,7,11,9,26,18,14,15};

  AVLTree<int> t;

  for(auto e : array)
  {
    t.Insert(e);
  }

  t.Inorder();
  cout<<endl;
  
  if(t.IsValidAVLTree())
  {
    cout<<"t is good VALTree"<<endl;
  }
  else
  {
    cout<<"t is bad VALTree"<<endl;
  }
}


int main()
{
  TestAVLTree();
  return 0;
}
