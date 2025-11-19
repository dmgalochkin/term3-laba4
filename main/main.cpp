#include <iostream>
#include "TMultiStack.hpp"

int main()
{
  std::cout << "=== TMultiStack Demo ===" << std::endl;
  
  std::cout << "\n1. Default constructor (empty multistack):" << std::endl;
  TMultiStack<int> emptyStack;
  std::cout << "Empty multistack - capacity: " << emptyStack.GetTotalCapacity() << ", stacks: " << emptyStack.GetStackCount() << std::endl;
  
  std::cout << "\n2. Parameterized constructor:" << std::endl;
  TMultiStack<int> multiStack(20, 3);
  
  std::cout << "Created multistack with 3 stacks, total capacity: " << multiStack.GetTotalCapacity() << std::endl;
  std::cout << "Stack 0 capacity: " << multiStack.GetCapacity(0) << std::endl;
  std::cout << "Stack 1 capacity: " << multiStack.GetCapacity(1) << std::endl;
  std::cout << "Stack 2 capacity: " << multiStack.GetCapacity(2) << std::endl;
  
  std::cout << "\nPushing elements to different stacks..." << std::endl;
  multiStack.Push(0, 10);
  multiStack.Push(0, 20);
  multiStack.Push(0, 30);
  
  multiStack.Push(1, 100);
  multiStack.Push(1, 200);
  
  multiStack.Push(2, 1000);
  multiStack.Push(2, 2000);
  multiStack.Push(2, 3000);
  multiStack.Push(2, 4000);
  
  std::cout << multiStack << std::endl;
  
  std::cout << "Stack 0 top: " << multiStack.Top(0) << std::endl;
  std::cout << "Stack 1 top: " << multiStack.Top(1) << std::endl;
  std::cout << "Stack 2 top: " << multiStack.Top(2) << std::endl;
}