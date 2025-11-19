#pragma once
#include <iostream>
#include <algorithm>
#include "TStack.hpp"

template<typename T>
class TMultiStack
{
private:
  int totalCapacity;
  int stackCount;
  T* memory;
  int* stackStarts;
  int* stackCapacities;
  int* stackSizes;
  void InitializeStacks();
  int FindTotalUsedSpace() const;
  void RepackInternal(int overflowStack);

public:
  TMultiStack();
  TMultiStack(int totalSize, int numStacks);
  TMultiStack(const TMultiStack<T>& other);
  TMultiStack(TMultiStack<T>&& other);
  ~TMultiStack();

  TMultiStack& operator=(const TMultiStack<T>& other);
  TMultiStack& operator=(TMultiStack<T>&& other);

  void Push(int stackIndex, const T& value);
  void Pop(int stackIndex);
  T& Top(int stackIndex);
  const T& Top(int stackIndex) const;

  bool IsEmpty(int stackIndex) const;
  bool IsFull(int stackIndex) const;
  int GetSize(int stackIndex) const;
  int GetCapacity(int stackIndex) const;
  int GetTotalCapacity() const;
  int GetStackCount() const;

  void Repack();

  bool operator==(const TMultiStack<T>& other) const;
  bool operator!=(const TMultiStack<T>& other) const;

  TStack<T> operator[](int stackIndex) const;

  template<typename U>
  friend std::ostream& operator<<(std::ostream& os, const TMultiStack<U>& multiStack);
  
  template<typename U>
  friend std::istream& operator>>(std::istream& is, TMultiStack<U>& multiStack);

  class Iterator
  {
  private:
    const TMultiStack<T>* multiStack;
    int currentStack;
    int currentPosition;

  public:
    Iterator(const TMultiStack<T>* ms, int stack, int pos);
    
    T& operator*();
    Iterator& operator++();
    Iterator operator++(int);
    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
  };

  Iterator begin() const;
  Iterator end() const;
};

template<typename T>
TMultiStack<T>::TMultiStack() : totalCapacity(0), stackCount(0), memory(nullptr), stackStarts(nullptr), stackCapacities(nullptr), stackSizes(nullptr)
{
}

template<typename T>
TMultiStack<T>::TMultiStack(int totalSize, int numStacks) : totalCapacity(totalSize), stackCount(numStacks)
{
  if (totalSize < 0)
  {
    throw "Negative total size";
  }
  if (numStacks <= 0)
  {
    throw "Number of stacks must be positive";
  }
  
  memory = new T[totalCapacity];
  stackStarts = new int[stackCount];
  stackCapacities = new int[stackCount];
  stackSizes = new int[stackCount];
  InitializeStacks();
}

template<typename T>
void TMultiStack<T>::InitializeStacks()
{
  int baseCapacity = totalCapacity / stackCount;
  int remainder = totalCapacity % stackCount;
  
  int currentPos = 0;
  for (int i = 0; i < stackCount; ++i)
  {
    stackStarts[i] = currentPos;
    stackCapacities[i] = baseCapacity + (i < remainder ? 1 : 0);
    stackSizes[i] = 0;
    currentPos += stackCapacities[i];
  }
}

template<typename T>
TMultiStack<T>::TMultiStack(const TMultiStack<T>& other) 
  : totalCapacity(other.totalCapacity), stackCount(other.stackCount)
{
  memory = new T[totalCapacity];
  stackStarts = new int[stackCount];
  stackCapacities = new int[stackCount];
  stackSizes = new int[stackCount];
  
  for (int i = 0; i < totalCapacity; ++i)
  {
    memory[i] = other.memory[i];
  }
  
  for (int i = 0; i < stackCount; ++i)
  {
    stackStarts[i] = other.stackStarts[i];
    stackCapacities[i] = other.stackCapacities[i];
    stackSizes[i] = other.stackSizes[i];
  }
}

template<typename T>
TMultiStack<T>::TMultiStack(TMultiStack<T>&& other)
  : totalCapacity(other.totalCapacity), stackCount(other.stackCount),
    memory(other.memory), stackStarts(other.stackStarts),
    stackCapacities(other.stackCapacities), stackSizes(other.stackSizes)
{
  other.memory = nullptr;
  other.stackStarts = nullptr;
  other.stackCapacities = nullptr;
  other.stackSizes = nullptr;
  other.totalCapacity = 0;
  other.stackCount = 0;
}

template<typename T>
TMultiStack<T>::~TMultiStack()
{
  delete[] memory;
  delete[] stackStarts;
  delete[] stackCapacities;
  delete[] stackSizes;
}

template<typename T>
TMultiStack<T>& TMultiStack<T>::operator=(const TMultiStack<T>& other)
{
  if (this == &other)
  {
    return *this;
  }
  
  delete[] memory;
  delete[] stackStarts;
  delete[] stackCapacities;
  delete[] stackSizes;
  
  totalCapacity = other.totalCapacity;
  stackCount = other.stackCount;
  
  memory = new T[totalCapacity];
  stackStarts = new int[stackCount];
  stackCapacities = new int[stackCount];
  stackSizes = new int[stackCount];
  
  for (int i = 0; i < totalCapacity; ++i)
  {
    memory[i] = other.memory[i];
  }
  
  for (int i = 0; i < stackCount; ++i)
  {
    stackStarts[i] = other.stackStarts[i];
    stackCapacities[i] = other.stackCapacities[i];
    stackSizes[i] = other.stackSizes[i];
  }
  
  return *this;
}

template<typename T>
TMultiStack<T>& TMultiStack<T>::operator=(TMultiStack<T>&& other)
{
  if (this == &other)
  {
    return *this;
  }
  
  delete[] memory;
  delete[] stackStarts;
  delete[] stackCapacities;
  delete[] stackSizes;
  
  totalCapacity = other.totalCapacity;
  stackCount = other.stackCount;
  memory = other.memory;
  stackStarts = other.stackStarts;
  stackCapacities = other.stackCapacities;
  stackSizes = other.stackSizes;
  
  other.memory = nullptr;
  other.stackStarts = nullptr;
  other.stackCapacities = nullptr;
  other.stackSizes = nullptr;
  other.totalCapacity = 0;
  other.stackCount = 0;
  
  return *this;
}

template<typename T>
void TMultiStack<T>::Push(int stackIndex, const T& value)
{
  if (stackIndex < 0 || stackIndex >= stackCount)
  {
    throw "Stack index out of range";
  }
  
  if (IsFull(stackIndex))
  {
    RepackInternal(stackIndex);
    if (IsFull(stackIndex))
    {
      int newCapacity = totalCapacity * 2;
      T* newMemory = new T[newCapacity];
      
      for (int i = 0; i < stackCount; ++i)
      {
        int oldStart = stackStarts[i];
        int size = stackSizes[i];
        for (int j = 0; j < size; ++j)
        {
          newMemory[oldStart + j] = memory[oldStart + j];
        }
      }
      
      delete[] memory;
      memory = newMemory;
      totalCapacity = newCapacity;
      
      RepackInternal(stackIndex);
      
      if (IsFull(stackIndex))
      {
        throw "All stacks are full";
      }
    }
  }
  
  int pos = stackStarts[stackIndex] + stackSizes[stackIndex];
  memory[pos] = value;
  stackSizes[stackIndex]++;
}

template<typename T>
void TMultiStack<T>::Pop(int stackIndex)
{
  if (stackIndex < 0 || stackIndex >= stackCount)
  {
    throw "Stack index out of range";
  }
  
  if (IsEmpty(stackIndex))
  {
    throw "Stack is empty";
  }
  
  stackSizes[stackIndex]--;
}

template<typename T>
T& TMultiStack<T>::Top(int stackIndex)
{
  if (stackIndex < 0 || stackIndex >= stackCount)
  {
    throw "Stack index out of range";
  }
  
  if (IsEmpty(stackIndex))
  {
    throw "Stack is empty";
  }
  
  int pos = stackStarts[stackIndex] + stackSizes[stackIndex] - 1;
  return memory[pos];
}

template<typename T>
const T& TMultiStack<T>::Top(int stackIndex) const
{
  if (stackIndex < 0 || stackIndex >= stackCount)
  {
    throw "Stack index out of range";
  }
  
  if (IsEmpty(stackIndex))
  {
    throw "Stack is empty";
  }
  
  int pos = stackStarts[stackIndex] + stackSizes[stackIndex] - 1;
  return memory[pos];
}

template<typename T>
bool TMultiStack<T>::IsEmpty(int stackIndex) const
{
  if (stackIndex < 0 || stackIndex >= stackCount)
  {
    throw "Stack index out of range";
  }
  return stackSizes[stackIndex] == 0;
}

template<typename T>
bool TMultiStack<T>::IsFull(int stackIndex) const
{
  if (stackIndex < 0 || stackIndex >= stackCount)
  {
    throw "Stack index out of range";
  }
  return stackSizes[stackIndex] == stackCapacities[stackIndex];
}

template<typename T>
int TMultiStack<T>::GetSize(int stackIndex) const
{
  if (stackIndex < 0 || stackIndex >= stackCount)
  {
    throw "Stack index out of range";
  }
  return stackSizes[stackIndex];
}

template<typename T>
int TMultiStack<T>::GetCapacity(int stackIndex) const
{
  if (stackIndex < 0 || stackIndex >= stackCount)
  {
    throw "Stack index out of range";
  }
  return stackCapacities[stackIndex];
}

template<typename T>
int TMultiStack<T>::GetTotalCapacity() const
{
  return totalCapacity;
}

template<typename T>
int TMultiStack<T>::GetStackCount() const
{
  return stackCount;
}

template<typename T>
int TMultiStack<T>::FindTotalUsedSpace() const
{
  int used = 0;
  for (int i = 0; i < stackCount; ++i)
  {
    used += stackSizes[i];
  }
  return used;
}

template<typename T>
void TMultiStack<T>::Repack()
{
  RepackInternal(-1);
}

template<typename T>
void TMultiStack<T>::RepackInternal(int overflowStack)
{
  int totalUsed = FindTotalUsedSpace();
  int freeSpace = totalCapacity - totalUsed;
  
  bool needExpansion = false;
  
  if (overflowStack >= 0 && IsFull(overflowStack))
  {
    if (freeSpace == 0)
    {
      needExpansion = true;
    }
  }
  
  if (needExpansion)
  {
    int newCapacity = totalCapacity * 2;
    T* newMemory = new T[newCapacity];
    
    for (int i = 0; i < stackCount; ++i)
    {
      int oldStart = stackStarts[i];
      int size = stackSizes[i];
      for (int j = 0; j < size; ++j)
      {
        newMemory[oldStart + j] = memory[oldStart + j];
      }
    }
    
    delete[] memory;
    memory = newMemory;
    totalCapacity = newCapacity;
    freeSpace = totalCapacity - totalUsed;
  }
  
  int* newStarts = new int[stackCount];
  int* newCapacities = new int[stackCount];
  
  int baseCapacity = totalCapacity / stackCount;
  int remainder = totalCapacity % stackCount;
  
  int totalAllocated = 0;
  for (int i = 0; i < stackCount; ++i)
  {
    newCapacities[i] = std::max(stackSizes[i], baseCapacity + (i < remainder ? 1 : 0));
    totalAllocated += newCapacities[i];
  }
  
  int extraSpace = totalCapacity - totalAllocated;
  if (overflowStack >= 0 && extraSpace > 0)
  {
    newCapacities[overflowStack] += extraSpace;
  }
  
  int currentPos = 0;
  for (int i = 0; i < stackCount; ++i)
  {
    newStarts[i] = currentPos;
    currentPos += newCapacities[i];
  }
  
  T* tempMemory = new T[totalUsed];
  int tempIndex = 0;
  
  for (int i = 0; i < stackCount; ++i)
  {
    int oldStart = stackStarts[i];
    int size = stackSizes[i];
    for (int j = 0; j < size; ++j)
    {
      tempMemory[tempIndex++] = memory[oldStart + j];
    }
  }
  
  tempIndex = 0;
  for (int i = 0; i < stackCount; ++i)
  {
    int newStart = newStarts[i];
    int size = stackSizes[i];
    for (int j = 0; j < size; ++j)
    {
      memory[newStart + j] = tempMemory[tempIndex++];
    }
    
    stackStarts[i] = newStart;
    stackCapacities[i] = newCapacities[i];
  }
  
  delete[] tempMemory;
  delete[] newStarts;
  delete[] newCapacities;
}

template<typename T>
bool TMultiStack<T>::operator==(const TMultiStack<T>& other) const
{
  if (totalCapacity != other.totalCapacity || stackCount != other.stackCount)
  {
    return false;
  }
  
  for (int i = 0; i < stackCount; ++i)
  {
    if (stackSizes[i] != other.stackSizes[i])
    {
      return false;
    }
    
    for (int j = 0; j < stackSizes[i]; ++j)
    {
      int pos1 = stackStarts[i] + j;
      int pos2 = other.stackStarts[i] + j;
      if (memory[pos1] != other.memory[pos2])
      {
        return false;
      }
    }
  }
  
  return true;
}

template<typename T>
bool TMultiStack<T>::operator!=(const TMultiStack<T>& other) const
{
  return !(*this == other);
}

template<typename T>
TStack<T> TMultiStack<T>::operator[](int stackIndex) const
{
  if (stackIndex < 0 || stackIndex >= stackCount)
  {
    throw "Stack index out of range";
  }
  
  TStack<T> result(stackCapacities[stackIndex]);
  for (int i = 0; i < stackSizes[stackIndex]; ++i)
  {
    int pos = stackStarts[stackIndex] + i;
    result.Push(memory[pos]);
  }
  
  return result;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const TMultiStack<T>& multiStack)
{
  os << "MultiStack[" << multiStack.stackCount << " stacks, total capacity: " << multiStack.totalCapacity << "]\n";
  for (int i = 0; i < multiStack.stackCount; ++i)
  {
    os << "  Stack " << i << " [" << multiStack.stackSizes[i] << "/" << multiStack.stackCapacities[i] << "]: ";
    if (multiStack.stackSizes[i] == 0)
    {
      os << "(empty)";
    }
    else
    {
      os << "[";
      for (int j = 0; j < multiStack.stackSizes[i]; ++j)
      {
        if (j > 0) os << ", ";
        int pos = multiStack.stackStarts[i] + j;
        os << multiStack.memory[pos];
      }
      os << "]";
    }
    os << "\n";
  }
  return os;
}

template<typename T>
std::istream& operator>>(std::istream& is, TMultiStack<T>& multiStack)
{
  int numStacks, totalSize;
  is >> numStacks >> totalSize;
  
  if (numStacks <= 0 || totalSize < 0)
  {
    throw "Sizes error";
  }
  
  TMultiStack<T> temp(totalSize, numStacks);
  
  for (int i = 0; i < numStacks; ++i)
  {
    int elementCount;
    is >> elementCount;
    for (int j = 0; j < elementCount; ++j)
    {
      T value;
      is >> value;
      temp.Push(i, value);
    }
  }
  
  multiStack = std::move(temp);
  return is;
}

template<typename T>
TMultiStack<T>::Iterator::Iterator(const TMultiStack<T>* ms, int stack, int pos)
  : multiStack(ms), currentStack(stack), currentPosition(pos)
{
}

template<typename T>
T& TMultiStack<T>::Iterator::operator*()
{
  if (currentStack >= multiStack->stackCount)
  {
    throw "Iterator out of bounds";
  }
  int pos = multiStack->stackStarts[currentStack] + currentPosition;
  return multiStack->memory[pos];
}

template<typename T>
typename TMultiStack<T>::Iterator& TMultiStack<T>::Iterator::operator++()
{
  currentPosition++;
  while (currentStack < multiStack->stackCount && 
         currentPosition >= multiStack->stackSizes[currentStack])
  {
    currentStack++;
    currentPosition = 0;
  }
  return *this;
}

template<typename T>
typename TMultiStack<T>::Iterator TMultiStack<T>::Iterator::operator++(int)
{
  Iterator temp = *this;
  ++(*this);
  return temp;
}

template<typename T>
bool TMultiStack<T>::Iterator::operator==(const Iterator& other) const
{
  return multiStack == other.multiStack && 
         currentStack == other.currentStack && 
         currentPosition == other.currentPosition;
}

template<typename T>
bool TMultiStack<T>::Iterator::operator!=(const Iterator& other) const
{
  return !(*this == other);
}

template<typename T>
typename TMultiStack<T>::Iterator TMultiStack<T>::begin() const
{
  for (int i = 0; i < stackCount; ++i)
  {
    if (stackSizes[i] > 0)
    {
      return Iterator(this, i, 0);
    }
  }
  return end();
}

template<typename T>
typename TMultiStack<T>::Iterator TMultiStack<T>::end() const
{
  return Iterator(this, stackCount, 0);
}