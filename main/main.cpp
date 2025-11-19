#include <iostream>
#include "TMultiStack.hpp"

int main()
{  
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
  
  std::cout << "\nPopping from stack 0..." << std::endl;
  std::cout << "Value before pop: " << multiStack.Top(0) << std::endl;
  multiStack.Pop(0);
  std::cout << "New stack 0 top: " << multiStack.Top(0) << std::endl;
  
  std::cout << "\nTesting memory expansion by filling stack 0..." << std::endl;
  for (int i = 0; i < 10; ++i)
  {
    multiStack.Push(0, 40 + i);
    std::cout << "Pushed " << (40 + i) << ", total capacity now: " << multiStack.GetTotalCapacity() << std::endl;
  }
  
  std::cout << "\nFinal state:" << std::endl;
  std::cout << multiStack << std::endl;
  
  std::cout << "\nTesting iterator..." << std::endl;
  std::cout << "All elements: ";
  for (auto it = multiStack.begin(); it != multiStack.end(); ++it)
  {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  
  std::cout << "\nTesting copy constructor..." << std::endl;
  TMultiStack<int> copy(multiStack);
  std::cout << "Copy created successfully" << std::endl;
  std::cout << "Original == Copy: " << (multiStack == copy ? "true" : "false") << std::endl;
  
  std::cout << "\nTesting with strings..." << std::endl;
  TMultiStack<std::string> stringStack(15, 2);
  stringStack.Push(0, "Hello");
  stringStack.Push(0, "World");
  stringStack.Push(1, "C++");
  stringStack.Push(1, "MultiStack");
  
  std::cout << stringStack << std::endl;
  return 0;
}