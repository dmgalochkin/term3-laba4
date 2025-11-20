#include <gtest/gtest.h>
#include "TQueue.hpp"

TEST(TQueueTest, DefaultConstructor)
{
  TQueue<int> queue;
  EXPECT_TRUE(queue.IsEmpty());
  EXPECT_TRUE(queue.IsFull());
  EXPECT_EQ(queue.GetSize(), 0);
  EXPECT_EQ(queue.GetCapacity(), 0);
}

TEST(TQueueTest, ConstructorWithCapacity)
{
  TQueue<int> queue(5);
  EXPECT_TRUE(queue.IsEmpty());
  EXPECT_FALSE(queue.IsFull());
  EXPECT_EQ(queue.GetSize(), 0);
  EXPECT_EQ(queue.GetCapacity(), 5);
}

TEST(TQueueTest, ConstructorWithZeroCapacity)
{
  TQueue<int> queue(0);
  EXPECT_TRUE(queue.IsEmpty());
  EXPECT_TRUE(queue.IsFull());
  EXPECT_EQ(queue.GetSize(), 0);
  EXPECT_EQ(queue.GetCapacity(), 0);
}

TEST(TQueueTest, ConstructorWithNegativeCapacity)
{
  EXPECT_THROW(TQueue<int> queue(-1), const char*);
}

TEST(TQueueTest, CopyConstructor)
{
  TQueue<int> original(3);
  original.Push(1);
  original.Push(2);
  original.Push(3);
  
  TQueue<int> copy(original);
  EXPECT_EQ(copy.GetSize(), 3);
  EXPECT_EQ(copy.GetCapacity(), 3);
  EXPECT_EQ(copy.Front(), 1);
  EXPECT_EQ(copy.Back(), 3);
  EXPECT_TRUE(copy == original);
}

TEST(TQueueTest, MoveConstructor)
{
  TQueue<int> original(3);
  original.Push(1);
  original.Push(2);
  original.Push(3);
  
  TQueue<int> moved(std::move(original));
  EXPECT_EQ(moved.GetSize(), 3);
  EXPECT_EQ(moved.GetCapacity(), 3);
  EXPECT_EQ(moved.Front(), 1);
  EXPECT_EQ(moved.Back(), 3);
}

TEST(TQueueTest, CopyAssignmentOperator)
{
  TQueue<int> original(3);
  original.Push(1);
  original.Push(2);
  
  TQueue<int> copy(5);
  copy = original;
  EXPECT_EQ(copy.GetSize(), 2);
  EXPECT_EQ(copy.GetCapacity(), 3);
  EXPECT_EQ(copy.Front(), 1);
  EXPECT_EQ(copy.Back(), 2);
  EXPECT_TRUE(copy == original);
}

TEST(TQueueTest, MoveAssignmentOperator)
{
  TQueue<int> original(3);
  original.Push(1);
  original.Push(2);
  
  TQueue<int> moved(5);
  moved = std::move(original);
  EXPECT_EQ(moved.GetSize(), 2);
  EXPECT_EQ(moved.GetCapacity(), 3);
  EXPECT_EQ(moved.Front(), 1);
  EXPECT_EQ(moved.Back(), 2);
}

TEST(TQueueTest, SelfAssignment)
{
  TQueue<int> queue(3);
  queue.Push(1);
  queue.Push(2);
  
  queue = queue;
  EXPECT_EQ(queue.GetSize(), 2);
  EXPECT_EQ(queue.GetCapacity(), 3);
  EXPECT_EQ(queue.Front(), 1);
  EXPECT_EQ(queue.Back(), 2);
}

TEST(TQueueTest, PushOperation)
{
  TQueue<int> queue(3);
  
  queue.Push(1);
  EXPECT_EQ(queue.GetSize(), 1);
  EXPECT_FALSE(queue.IsEmpty());
  EXPECT_EQ(queue.Front(), 1);
  EXPECT_EQ(queue.Back(), 1);
  
  queue.Push(2);
  EXPECT_EQ(queue.GetSize(), 2);
  EXPECT_EQ(queue.Front(), 1);
  EXPECT_EQ(queue.Back(), 2);
  
  queue.Push(3);
  EXPECT_EQ(queue.GetSize(), 3);
  EXPECT_TRUE(queue.IsFull());
  EXPECT_EQ(queue.Front(), 1);
  EXPECT_EQ(queue.Back(), 3);
}

TEST(TQueueTest, PushOnFullQueue)
{
  TQueue<int> queue(2);
  queue.Push(1);
  queue.Push(2);
  
  EXPECT_THROW(queue.Push(3), const char*);
}

TEST(TQueueTest, PopOperation)
{
  TQueue<int> queue(3);
  queue.Push(1);
  queue.Push(2);
  queue.Push(3);
  
  queue.Pop();
  EXPECT_EQ(queue.GetSize(), 2);
  EXPECT_EQ(queue.Front(), 2);
  EXPECT_EQ(queue.Back(), 3);
  
  queue.Pop();
  EXPECT_EQ(queue.GetSize(), 1);
  EXPECT_EQ(queue.Front(), 3);
  EXPECT_EQ(queue.Back(), 3);
  
  queue.Pop();
  EXPECT_EQ(queue.GetSize(), 0);
  EXPECT_TRUE(queue.IsEmpty());
}

TEST(TQueueTest, PopOnEmptyQueue)
{
  TQueue<int> queue(3);
  EXPECT_THROW(queue.Pop(), const char*);
}

TEST(TQueueTest, FrontOnEmptyQueue)
{
  TQueue<int> queue(3);
  EXPECT_THROW(queue.Front(), const char*);
}

TEST(TQueueTest, BackOnEmptyQueue)
{
  TQueue<int> queue(3);
  EXPECT_THROW(queue.Back(), const char*);
}

TEST(TQueueTest, FrontBackReferenceModification)
{
  TQueue<int> queue(3);
  queue.Push(1);
  queue.Push(2);
  queue.Push(3);
  
  queue.Front() = 10;
  EXPECT_EQ(queue.Front(), 10);
  
  queue.Back() = 30;
  EXPECT_EQ(queue.Back(), 30);
}

TEST(TQueueTest, EqualityOperators)
{
  TQueue<int> queue1(3);
  TQueue<int> queue2(3);
  
  EXPECT_TRUE(queue1 == queue2);
  EXPECT_FALSE(queue1 != queue2);
  
  queue1.Push(1);
  EXPECT_FALSE(queue1 == queue2);
  EXPECT_TRUE(queue1 != queue2);
  
  queue2.Push(1);
  EXPECT_TRUE(queue1 == queue2);
  EXPECT_FALSE(queue1 != queue2);
  
  queue1.Push(2);
  queue2.Push(3);
  EXPECT_FALSE(queue1 == queue2);
  EXPECT_TRUE(queue1 != queue2);
}

TEST(TQueueTest, ResizeOperation)
{
  TQueue<int> queue(3);
  queue.Push(1);
  queue.Push(2);
  
  queue.Resize(5);
  EXPECT_EQ(queue.GetCapacity(), 5);
  EXPECT_EQ(queue.GetSize(), 2);
  EXPECT_EQ(queue.Front(), 1);
  EXPECT_EQ(queue.Back(), 2);
  
  queue.Push(3);
  queue.Push(4);
  EXPECT_EQ(queue.GetSize(), 4);
  EXPECT_EQ(queue.Front(), 1);
  EXPECT_EQ(queue.Back(), 4);
  
  queue.Resize(2);
  EXPECT_EQ(queue.GetCapacity(), 2);
  EXPECT_EQ(queue.GetSize(), 2);
  EXPECT_EQ(queue.Front(), 1);
  EXPECT_EQ(queue.Back(), 2);
}

TEST(TQueueTest, ResizeToZero)
{
  TQueue<int> queue(3);
  queue.Push(1);
  queue.Push(2);
  
  queue.Resize(0);
  EXPECT_EQ(queue.GetCapacity(), 0);
  EXPECT_EQ(queue.GetSize(), 0);
  EXPECT_TRUE(queue.IsEmpty());
  EXPECT_TRUE(queue.IsFull());
}

TEST(TQueueTest, ResizeWithNegativeCapacity)
{
  TQueue<int> queue(3);
  EXPECT_THROW(queue.Resize(-1), const char*);
}

TEST(TQueueTest, ResizeToSameCapacity)
{
  TQueue<int> queue(3);
  queue.Push(1);
  queue.Push(2);
  
  queue.Resize(3);
  EXPECT_EQ(queue.GetCapacity(), 3);
  EXPECT_EQ(queue.GetSize(), 2);
  EXPECT_EQ(queue.Front(), 1);
  EXPECT_EQ(queue.Back(), 2);
}

TEST(TQueueTest, DifferentDataTypes)
{
  TQueue<std::string> stringQueue(2);
  stringQueue.Push("hello");
  stringQueue.Push("world");
  
  EXPECT_EQ(stringQueue.Front(), "hello");
  EXPECT_EQ(stringQueue.Back(), "world");
  stringQueue.Pop();
  EXPECT_EQ(stringQueue.Front(), "world");
  EXPECT_EQ(stringQueue.Back(), "world");
  
  TQueue<double> doubleQueue(2);
  doubleQueue.Push(3.14);
  doubleQueue.Push(2.71);
  
  EXPECT_DOUBLE_EQ(doubleQueue.Front(), 3.14);
  EXPECT_DOUBLE_EQ(doubleQueue.Back(), 2.71);
  doubleQueue.Pop();
  EXPECT_DOUBLE_EQ(doubleQueue.Front(), 2.71);
  EXPECT_DOUBLE_EQ(doubleQueue.Back(), 2.71);
}

TEST(TQueueTest, FIFOBehavior)
{
  TQueue<int> queue(5);
  
  for (int i = 1; i <= 5; ++i)
  {
    queue.Push(i);
  }
  
  for (int i = 1; i <= 5; ++i)
  {
    EXPECT_EQ(queue.Front(), i);
    queue.Pop();
  }
  
  EXPECT_TRUE(queue.IsEmpty());
}

TEST(TQueueTest, CircularBufferBehavior)
{
  TQueue<int> queue(3);
  
  queue.Push(1);
  queue.Push(2);
  queue.Push(3);
  
  queue.Pop();
  EXPECT_EQ(queue.Front(), 2);
  EXPECT_EQ(queue.Back(), 3);
  
  queue.Push(4);
  EXPECT_EQ(queue.Front(), 2);
  EXPECT_EQ(queue.Back(), 4);
  
  queue.Pop();
  queue.Push(5);
  EXPECT_EQ(queue.Front(), 3);
  EXPECT_EQ(queue.Back(), 5);
  
  queue.Pop();
  queue.Push(6);
  EXPECT_EQ(queue.Front(), 4);
  EXPECT_EQ(queue.Back(), 6);
}

TEST(TQueueTest, EqualityWithCircularBuffer)
{
  TQueue<int> queue1(4);
  TQueue<int> queue2(4);
  
  queue1.Push(1);
  queue1.Push(2);
  queue1.Push(3);
  
  queue2.Push(1);
  queue2.Push(2);
  queue2.Push(3);
  
  EXPECT_TRUE(queue1 == queue2);
  
  queue1.Pop();
  queue1.Push(4);
  
  queue2.Pop();
  queue2.Push(4);
  
  EXPECT_TRUE(queue1 == queue2);
  EXPECT_EQ(queue1.Front(), 2);
  EXPECT_EQ(queue1.Back(), 4);
  EXPECT_EQ(queue2.Front(), 2);
  EXPECT_EQ(queue2.Back(), 4);
}

TEST(TQueueTest, CopyConstructorWithCircularBuffer)
{
  TQueue<int> original(4);
  original.Push(1);
  original.Push(2);
  original.Push(3);
  original.Push(4);
  
  original.Pop();
  original.Pop();
  original.Push(5);
  original.Push(6);
  
  TQueue<int> copy(original);
  EXPECT_TRUE(copy == original);
  EXPECT_EQ(copy.Front(), 3);
  EXPECT_EQ(copy.Back(), 6);
  EXPECT_EQ(copy.GetSize(), 4);
}