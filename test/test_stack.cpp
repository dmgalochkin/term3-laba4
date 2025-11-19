#include <gtest/gtest.h>
#include "TStack.hpp"

TEST(TStackTest, DefaultConstructor)
{
  TStack<int> stack;
  EXPECT_TRUE(stack.IsEmpty());
  EXPECT_TRUE(stack.IsFull());
  EXPECT_EQ(stack.GetSize(), 0);
  EXPECT_EQ(stack.GetCapacity(), 0);
}

TEST(TStackTest, ConstructorWithCapacity)
{
  TStack<int> stack(5);
  EXPECT_TRUE(stack.IsEmpty());
  EXPECT_FALSE(stack.IsFull());
  EXPECT_EQ(stack.GetSize(), 0);
  EXPECT_EQ(stack.GetCapacity(), 5);
}

TEST(TStackTest, ConstructorWithZeroCapacity)
{
  TStack<int> stack(0);
  EXPECT_TRUE(stack.IsEmpty());
  EXPECT_TRUE(stack.IsFull());
  EXPECT_EQ(stack.GetSize(), 0);
  EXPECT_EQ(stack.GetCapacity(), 0);
}

TEST(TStackTest, ConstructorWithNegativeCapacity)
{
  EXPECT_THROW(TStack<int> stack(-1), const char*);
}

TEST(TStackTest, CopyConstructor)
{
  TStack<int> original(3);
  original.Push(1);
  original.Push(2);
  original.Push(3);
  
  TStack<int> copy(original);
  EXPECT_EQ(copy.GetSize(), 3);
  EXPECT_EQ(copy.GetCapacity(), 3);
  EXPECT_EQ(copy.Top(), 3);
  EXPECT_TRUE(copy == original);
}

TEST(TStackTest, MoveConstructor)
{
  TStack<int> original(3);
  original.Push(1);
  original.Push(2);
  original.Push(3);
  
  TStack<int> moved(std::move(original));
  EXPECT_EQ(moved.GetSize(), 3);
  EXPECT_EQ(moved.GetCapacity(), 3);
  EXPECT_EQ(moved.Top(), 3);
}

TEST(TStackTest, CopyAssignmentOperator)
{
  TStack<int> original(3);
  original.Push(1);
  original.Push(2);
  
  TStack<int> copy(5);
  copy = original;
  EXPECT_EQ(copy.GetSize(), 2);
  EXPECT_EQ(copy.GetCapacity(), 3);
  EXPECT_EQ(copy.Top(), 2);
  EXPECT_TRUE(copy == original);
}

TEST(TStackTest, MoveAssignmentOperator)
{
  TStack<int> original(3);
  original.Push(1);
  original.Push(2);
  
  TStack<int> moved(5);
  moved = std::move(original);
  EXPECT_EQ(moved.GetSize(), 2);
  EXPECT_EQ(moved.GetCapacity(), 3);
  EXPECT_EQ(moved.Top(), 2);
}

TEST(TStackTest, SelfAssignment)
{
  TStack<int> stack(3);
  stack.Push(1);
  stack.Push(2);
  
  stack = stack;
  EXPECT_EQ(stack.GetSize(), 2);
  EXPECT_EQ(stack.GetCapacity(), 3);
  EXPECT_EQ(stack.Top(), 2);
}

TEST(TStackTest, PushOperation)
{
  TStack<int> stack(3);
  
  stack.Push(1);
  EXPECT_EQ(stack.GetSize(), 1);
  EXPECT_FALSE(stack.IsEmpty());
  EXPECT_EQ(stack.Top(), 1);
  
  stack.Push(2);
  EXPECT_EQ(stack.GetSize(), 2);
  EXPECT_EQ(stack.Top(), 2);
  
  stack.Push(3);
  EXPECT_EQ(stack.GetSize(), 3);
  EXPECT_TRUE(stack.IsFull());
  EXPECT_EQ(stack.Top(), 3);
}

TEST(TStackTest, PushOnFullStack)
{
  TStack<int> stack(2);
  stack.Push(1);
  stack.Push(2);
  
  EXPECT_THROW(stack.Push(3), const char*);
}

TEST(TStackTest, PopOperation)
{
  TStack<int> stack(3);
  stack.Push(1);
  stack.Push(2);
  stack.Push(3);
  
  stack.Pop();
  EXPECT_EQ(stack.GetSize(), 2);
  EXPECT_EQ(stack.Top(), 2);
  
  stack.Pop();
  EXPECT_EQ(stack.GetSize(), 1);
  EXPECT_EQ(stack.Top(), 1);
  
  stack.Pop();
  EXPECT_EQ(stack.GetSize(), 0);
  EXPECT_TRUE(stack.IsEmpty());
}

TEST(TStackTest, PopOnEmptyStack)
{
  TStack<int> stack(3);
  EXPECT_THROW(stack.Pop(), const char*);
}

TEST(TStackTest, TopOnEmptyStack)
{
  TStack<int> stack(3);
  EXPECT_THROW(stack.Top(), const char*);
}

TEST(TStackTest, TopReferenceModification)
{
  TStack<int> stack(3);
  stack.Push(1);
  stack.Push(2);
  
  stack.Top() = 5;
  EXPECT_EQ(stack.Top(), 5);
}

TEST(TStackTest, EqualityOperators)
{
  TStack<int> stack1(3);
  TStack<int> stack2(3);
  
  EXPECT_TRUE(stack1 == stack2);
  EXPECT_FALSE(stack1 != stack2);
  
  stack1.Push(1);
  EXPECT_FALSE(stack1 == stack2);
  EXPECT_TRUE(stack1 != stack2);
  
  stack2.Push(1);
  EXPECT_TRUE(stack1 == stack2);
  EXPECT_FALSE(stack1 != stack2);
  
  stack1.Push(2);
  stack2.Push(3);
  EXPECT_FALSE(stack1 == stack2);
  EXPECT_TRUE(stack1 != stack2);
}

TEST(TStackTest, ResizeOperation)
{
  TStack<int> stack(3);
  stack.Push(1);
  stack.Push(2);
  
  stack.Resize(5);
  EXPECT_EQ(stack.GetCapacity(), 5);
  EXPECT_EQ(stack.GetSize(), 2);
  EXPECT_EQ(stack.Top(), 2);
  
  stack.Push(3);
  stack.Push(4);
  EXPECT_EQ(stack.GetSize(), 4);
  
  stack.Resize(2);
  EXPECT_EQ(stack.GetCapacity(), 2);
  EXPECT_EQ(stack.GetSize(), 2);
  EXPECT_EQ(stack.Top(), 2);
}

TEST(TStackTest, ResizeToZero)
{
  TStack<int> stack(3);
  stack.Push(1);
  stack.Push(2);
  
  stack.Resize(0);
  EXPECT_EQ(stack.GetCapacity(), 0);
  EXPECT_EQ(stack.GetSize(), 0);
  EXPECT_TRUE(stack.IsEmpty());
  EXPECT_TRUE(stack.IsFull());
}

TEST(TStackTest, ResizeWithNegativeCapacity)
{
  TStack<int> stack(3);
  EXPECT_THROW(stack.Resize(-1), const char*);
}

TEST(TStackTest, ResizeToSameCapacity)
{
  TStack<int> stack(3);
  stack.Push(1);
  stack.Push(2);
  
  stack.Resize(3);
  EXPECT_EQ(stack.GetCapacity(), 3);
  EXPECT_EQ(stack.GetSize(), 2);
  EXPECT_EQ(stack.Top(), 2);
}

TEST(TStackTest, DifferentDataTypes)
{
  TStack<std::string> stringStack(2);
  stringStack.Push("hello");
  stringStack.Push("world");
  
  EXPECT_EQ(stringStack.Top(), "world");
  stringStack.Pop();
  EXPECT_EQ(stringStack.Top(), "hello");
  
  TStack<double> doubleStack(2);
  doubleStack.Push(3.14);
  doubleStack.Push(2.71);
  
  EXPECT_DOUBLE_EQ(doubleStack.Top(), 2.71);
  doubleStack.Pop();
  EXPECT_DOUBLE_EQ(doubleStack.Top(), 3.14);
}

TEST(TStackTest, LIFOBehavior)
{
  TStack<int> stack(5);
  
  for (int i = 1; i <= 5; ++i)
  {
    stack.Push(i);
  }
  
  for (int i = 5; i >= 1; --i)
  {
    EXPECT_EQ(stack.Top(), i);
    stack.Pop();
  }
  
  EXPECT_TRUE(stack.IsEmpty());
}