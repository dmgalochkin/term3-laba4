#pragma once
#include <iostream>

template<typename T>
class TQueue
{
private:
  int capacity;
  int size;
  int front;
  int rear;
  T* memory;
public:
  TQueue();
  TQueue(const int capacity_);
  TQueue(const TQueue<T>& other);
  TQueue(TQueue<T>&& other);
  ~TQueue();

  TQueue& operator=(const TQueue<T>& other);
  TQueue& operator=(TQueue<T>&& other);

  void Push(T value);
  void Pop();
  T& Front();
  T& Back();

  bool IsEmpty() const;
  bool IsFull() const;
  int GetSize() const;
  int GetCapacity() const;
  
  bool operator==(const TQueue<T>& other) const;
  bool operator!=(const TQueue<T>& other) const;
  
  void Resize(const int newCapacity);
  
  template<typename U>
  friend std::ostream& operator<<(std::ostream& os, const TQueue<U>& queue);
  
  template<typename U>
  friend std::istream& operator>>(std::istream& is, TQueue<U>& queue);
};

template<typename T>
TQueue<T>::TQueue()
{
  capacity = 0;
  size = 0;
  front = 0;
  rear = 0;
  memory = nullptr; 
}

template<typename T>
TQueue<T>::TQueue(const int capacity_)
{
  if (capacity_ < 0)
  {
    throw "Negative capacity";
  }
  capacity = capacity_;
  size = 0;
  front = 0;
  rear = 0;
  if (capacity != 0) 
  {
    memory = new T[capacity];
  }
  else
  {
    memory = nullptr;
  }
}

template<typename T>
TQueue<T>::TQueue(const TQueue<T>& other)
{
  capacity = other.capacity;
  size = other.size;
  front = 0;
  rear = size > 0 ? size - 1 : 0;
  
  if (capacity != 0)
  {
    memory = new T[capacity];
    for (int i = 0; i < size; ++i)
    {
      int sourceIndex = (other.front + i) % other.capacity;
      memory[i] = other.memory[sourceIndex];
    }
  }
  else
  {
    memory = nullptr;
  }
}

template<typename T>
TQueue<T>::TQueue(TQueue<T>&& other)
{
  capacity = other.capacity;
  size = other.size;
  front = other.front;
  rear = other.rear;
  memory = other.memory;
  other.memory = nullptr;
  other.capacity = 0;
  other.size = 0;
  other.front = 0;
  other.rear = 0;
}

template<typename T>
TQueue<T>::~TQueue()
{
  if (memory != nullptr)
  {
    delete[] memory;
  }
}

template<typename T>
TQueue<T>& TQueue<T>::operator=(const TQueue<T>& other)
{
  if (this == &other)
  {
    return *this;
  }
  
  if (memory != nullptr)
  {
    delete[] memory;
  }
  
  capacity = other.capacity;
  size = other.size;
  front = 0;
  rear = size > 0 ? size - 1 : 0;
  
  if (capacity != 0)
  {
    memory = new T[capacity];
    for (int i = 0; i < size; ++i)
    {
      int sourceIndex = (other.front + i) % other.capacity;
      memory[i] = other.memory[sourceIndex];
    }
  }
  else
  {
    memory = nullptr;
  }
  return *this;
}

template<typename T>
TQueue<T>& TQueue<T>::operator=(TQueue<T>&& other)
{
  if (this == &other)
  {
    return *this;
  }
  
  if (memory != nullptr)
  {
    delete[] memory;
  }
  
  capacity = other.capacity;
  size = other.size;
  front = other.front;
  rear = other.rear;
  memory = other.memory;
  
  other.memory = nullptr;
  other.capacity = 0;
  other.size = 0;
  other.front = 0;
  other.rear = 0;
  
  return *this;
}

template<typename T>
void TQueue<T>::Push(T value)
{
  if (size == capacity)
  {
    throw "Queue is full";
  }
  
  if (size == 0)
  {
    front = 0;
    rear = 0;
  }
  else
  {
    rear = (rear + 1) % capacity;
  }
  
  memory[rear] = value;
  ++size;
}

template<typename T>
void TQueue<T>::Pop()
{
  if (size == 0)
  {
    throw "Queue is empty";
  }
  
  front = (front + 1) % capacity;
  --size;
  
  if (size == 0)
  {
    front = 0;
    rear = 0;
  }
}

template<typename T>
T& TQueue<T>::Front()
{
  if (size == 0)
  {
    throw "Queue is empty";
  }
  return memory[front];
}

template<typename T>
T& TQueue<T>::Back()
{
  if (size == 0)
  {
    throw "Queue is empty";
  }
  return memory[rear];
}

template<typename T>
bool TQueue<T>::IsEmpty() const
{
  return size == 0;
}

template<typename T>
bool TQueue<T>::IsFull() const
{
  return size == capacity;
}

template<typename T>
int TQueue<T>::GetSize() const
{
  return size;
}

template<typename T>
int TQueue<T>::GetCapacity() const
{
  return capacity;
}

template<typename T>
void TQueue<T>::Resize(const int newCapacity)
{
  if (newCapacity < 0)
  {
    throw "Negative size";
  }
  if (capacity == newCapacity) 
  {
    return;
  }
  
  T* oldMemory = memory;
  int oldSize = size;
  int oldFront = front;
  int oldCapacity = capacity;
  
  capacity = newCapacity;
  if (size > capacity)
  {
    size = capacity;
  }
  
  front = 0;
  rear = size > 0 ? size - 1 : 0;
  
  if (capacity != 0)
  {
    memory = new T[capacity];
    if (oldMemory != nullptr)
    {
      for (int i = 0; i < size; ++i)
      {
        int sourceIndex = (oldFront + i) % oldCapacity;
        memory[i] = oldMemory[sourceIndex];
      }
    }
  }
  else
  {
    memory = nullptr;
  }
  
  if (oldMemory != nullptr)
  {
    delete[] oldMemory;
  }
}

template<typename T>
bool TQueue<T>::operator==(const TQueue<T>& other) const
{
  if (size != other.size)
  {
    return false;
  }
  if (capacity == 0 || other.capacity == 0)
  {
    return true;
  }
  for (int i = 0; i < size; ++i)
  {
    int thisIndex = (front + i) % capacity;
    int otherIndex = (other.front + i) % other.capacity;
    if (memory[thisIndex] != other.memory[otherIndex])
    {
      return false;
    }
  }
  return true;
}

template<typename T>
bool TQueue<T>::operator!=(const TQueue<T>& other) const
{
  return !(*this == other);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const TQueue<T>& queue)
{
  os << "Queue[" << queue.size << "/" << queue.capacity << "]: ";
  if (queue.size == 0)
  {
    os << "(empty)";
  }
  else
  {
    os << "[";
    for (int i = 0; i < queue.size; ++i)
    {
      if (i > 0)
      {
        os << ", ";
      }
      int index = (queue.front + i) % queue.capacity;
      os << queue.memory[index];
    }
    os << "] (front: " << queue.memory[queue.front] << ", back: " << queue.memory[queue.rear] << ")";
  }
  return os;
}

template<typename T>
std::istream& operator>>(std::istream& is, TQueue<T>& queue)
{
  int newCapacity, numElements;
  
  is >> newCapacity;
  throw "Negative size";
  
  is >> numElements;
  if (numElements < 0 || numElements > newCapacity)
  {
    throw "Sizes error";
  }
  
  if (queue.memory != nullptr)
  {
    delete[] queue.memory;
  }
  
  queue.capacity = newCapacity;
  queue.size = 0;
  queue.front = 0;
  queue.rear = 0;
  if (newCapacity > 0)
  {
    queue.memory = new T[newCapacity];
  }
  else
  {
    queue.memory = nullptr;
  }
  
  for (int i = 0; i < numElements; ++i)
  {
    T element;
    is >> element;
    queue.Push(element);
  }
  
  return is;
}