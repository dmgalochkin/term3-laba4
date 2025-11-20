#include <gtest/gtest.h>
#include "TMultiStack.hpp"
#include <vector>

TEST(TMultiStackTest, DefaultConstructor)
{
    TMultiStack<int> multiStack;
    EXPECT_EQ(multiStack.GetTotalCapacity(), 0);
    EXPECT_EQ(multiStack.GetStackCount(), 0);
}

TEST(TMultiStackTest, ConstructorWithParameters)
{
    TMultiStack<int> multiStack(50, 5);
    EXPECT_EQ(multiStack.GetTotalCapacity(), 50);
    EXPECT_EQ(multiStack.GetStackCount(), 5);
    
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_TRUE(multiStack.IsEmpty(i));
        EXPECT_FALSE(multiStack.IsFull(i));
        EXPECT_EQ(multiStack.GetSize(i), 0);
    }
}

TEST(TMultiStackTest, ConstructorWithInvalidParameters)
{
    ASSERT_ANY_THROW(TMultiStack<int> multiStack(-10, 3));
    ASSERT_ANY_THROW(TMultiStack<int> multiStack(100, 0));
    ASSERT_ANY_THROW(TMultiStack<int> multiStack(100, -1));
}

TEST(TMultiStackTest, CopyConstructor)
{
    TMultiStack<int> original(30, 3);
    original.Push(0, 1);
    original.Push(0, 2);
    original.Push(1, 10);
    original.Push(2, 20);
    original.Push(2, 21);
    
    TMultiStack<int> copy(original);
    EXPECT_EQ(copy.GetTotalCapacity(), 30);
    EXPECT_EQ(copy.GetStackCount(), 3);
    EXPECT_EQ(copy.GetSize(0), 2);
    EXPECT_EQ(copy.GetSize(1), 1);
    EXPECT_EQ(copy.GetSize(2), 2);
    EXPECT_TRUE(copy == original);
}

TEST(TMultiStackTest, MoveConstructor)
{
    TMultiStack<int> original(30, 3);
    original.Push(0, 1);
    original.Push(1, 10);
    original.Push(2, 20);
    
    TMultiStack<int> moved(std::move(original));
    EXPECT_EQ(moved.GetTotalCapacity(), 30);
    EXPECT_EQ(moved.GetStackCount(), 3);
    EXPECT_EQ(moved.GetSize(0), 1);
    EXPECT_EQ(moved.GetSize(1), 1);
    EXPECT_EQ(moved.GetSize(2), 1);
}

TEST(TMultiStackTest, CopyAssignmentOperator)
{
    TMultiStack<int> original(30, 3);
    original.Push(0, 1);
    original.Push(1, 10);
    
    TMultiStack<int> copy(50, 5);
    copy = original;
    EXPECT_EQ(copy.GetTotalCapacity(), 30);
    EXPECT_EQ(copy.GetStackCount(), 3);
    EXPECT_TRUE(copy == original);
}

TEST(TMultiStackTest, MoveAssignmentOperator)
{
    TMultiStack<int> original(30, 3);
    original.Push(0, 1);
    original.Push(1, 10);
    
    TMultiStack<int> moved(50, 5);
    moved = std::move(original);
    EXPECT_EQ(moved.GetTotalCapacity(), 30);
    EXPECT_EQ(moved.GetStackCount(), 3);
    EXPECT_EQ(moved.GetSize(0), 1);
    EXPECT_EQ(moved.GetSize(1), 1);
}

TEST(TMultiStackTest, SelfAssignment)
{
    TMultiStack<int> multiStack(30, 3);
    multiStack.Push(0, 1);
    multiStack.Push(1, 10);
    
    multiStack = multiStack;
    EXPECT_EQ(multiStack.GetTotalCapacity(), 30);
    EXPECT_EQ(multiStack.GetStackCount(), 3);
    EXPECT_EQ(multiStack.GetSize(0), 1);
    EXPECT_EQ(multiStack.GetSize(1), 1);
}

TEST(TMultiStackTest, PushOperation)
{
    TMultiStack<int> multiStack(30, 3);
    
    multiStack.Push(0, 1);
    EXPECT_EQ(multiStack.GetSize(0), 1);
    EXPECT_FALSE(multiStack.IsEmpty(0));
    EXPECT_EQ(multiStack.Top(0), 1);
    
    multiStack.Push(0, 2);
    EXPECT_EQ(multiStack.GetSize(0), 2);
    EXPECT_EQ(multiStack.Top(0), 2);
    
    multiStack.Push(1, 10);
    EXPECT_EQ(multiStack.GetSize(1), 1);
    EXPECT_EQ(multiStack.Top(1), 10);
}

TEST(TMultiStackTest, PushWithInvalidIndex)
{
    TMultiStack<int> multiStack(30, 3);
    ASSERT_ANY_THROW(multiStack.Push(-1, 1));
    ASSERT_ANY_THROW(multiStack.Push(3, 1));
}

TEST(TMultiStackTest, PopOperation)
{
    TMultiStack<int> multiStack(30, 3);
    multiStack.Push(0, 1);
    multiStack.Push(0, 2);
    multiStack.Push(0, 3);
    
    EXPECT_EQ(multiStack.Top(0), 3);
    multiStack.Pop(0);
    EXPECT_EQ(multiStack.GetSize(0), 2);
    EXPECT_EQ(multiStack.Top(0), 2);
    
    EXPECT_EQ(multiStack.Top(0), 2);
    multiStack.Pop(0);
    EXPECT_EQ(multiStack.GetSize(0), 1);
    EXPECT_EQ(multiStack.Top(0), 1);
    
    EXPECT_EQ(multiStack.Top(0), 1);
    multiStack.Pop(0);
    EXPECT_EQ(multiStack.GetSize(0), 0);
    EXPECT_TRUE(multiStack.IsEmpty(0));
}

TEST(TMultiStackTest, PopWithInvalidIndex)
{
    TMultiStack<int> multiStack(30, 3);
    ASSERT_ANY_THROW(multiStack.Pop(-1));
    ASSERT_ANY_THROW(multiStack.Pop(3));
}

TEST(TMultiStackTest, PopOnEmptyStack)
{
    TMultiStack<int> multiStack(30, 3);
    ASSERT_ANY_THROW(multiStack.Pop(0));
}

TEST(TMultiStackTest, TopOperation)
{
    TMultiStack<int> multiStack(30, 3);
    multiStack.Push(0, 1);
    multiStack.Push(0, 2);
    
    EXPECT_EQ(multiStack.Top(0), 2);
    multiStack.Top(0) = 5;
    EXPECT_EQ(multiStack.Top(0), 5);
}

TEST(TMultiStackTest, TopWithInvalidIndex)
{
    TMultiStack<int> multiStack(30, 3);
    multiStack.Push(0, 1);
    ASSERT_ANY_THROW(multiStack.Top(-1));
    ASSERT_ANY_THROW(multiStack.Top(3));
}

TEST(TMultiStackTest, TopOnEmptyStack)
{
    TMultiStack<int> multiStack(30, 3);
    ASSERT_ANY_THROW(multiStack.Top(0));
}

TEST(TMultiStackTest, StatusChecks)
{
    TMultiStack<int> multiStack(6, 3);
    
    EXPECT_TRUE(multiStack.IsEmpty(0));
    EXPECT_FALSE(multiStack.IsFull(0));
    
    multiStack.Push(0, 1);
    EXPECT_FALSE(multiStack.IsEmpty(0));
    EXPECT_FALSE(multiStack.IsFull(0));
    
    multiStack.Push(0, 2);
    EXPECT_FALSE(multiStack.IsEmpty(0));
    EXPECT_TRUE(multiStack.IsFull(0));
}

TEST(TMultiStackTest, StatusChecksWithInvalidIndex)
{
    TMultiStack<int> multiStack(30, 3);
    ASSERT_ANY_THROW(multiStack.IsEmpty(-1));
    ASSERT_ANY_THROW(multiStack.IsEmpty(3));
    ASSERT_ANY_THROW(multiStack.IsFull(-1));
    ASSERT_ANY_THROW(multiStack.IsFull(3));
}

TEST(TMultiStackTest, GettersWithInvalidIndex)
{
    TMultiStack<int> multiStack(30, 3);
    ASSERT_ANY_THROW(multiStack.GetSize(-1));
    ASSERT_ANY_THROW(multiStack.GetSize(3));
    ASSERT_ANY_THROW(multiStack.GetCapacity(-1));
    ASSERT_ANY_THROW(multiStack.GetCapacity(3));
}

TEST(TMultiStackTest, RepackOperation)
{
    TMultiStack<int> multiStack(9, 3);
    
    multiStack.Push(0, 1);
    multiStack.Push(0, 2);
    multiStack.Push(0, 3);
    EXPECT_TRUE(multiStack.IsFull(0));
    
    multiStack.Push(1, 10);
    multiStack.Push(2, 20);
    
    multiStack.Pop(1);
    
    multiStack.Push(0, 4);
    EXPECT_EQ(multiStack.GetSize(0), 4);
    EXPECT_EQ(multiStack.Top(0), 4);
}

TEST(TMultiStackTest, ManualRepack)
{
    TMultiStack<int> multiStack(12, 3);
    multiStack.Push(0, 1);
    multiStack.Push(1, 10);
    multiStack.Push(2, 20);
    
    int oldCapacity0 = multiStack.GetCapacity(0);
    multiStack.Repack();
    
    EXPECT_EQ(multiStack.GetSize(0), 1);
    EXPECT_EQ(multiStack.GetSize(1), 1);
    EXPECT_EQ(multiStack.GetSize(2), 1);
    EXPECT_EQ(multiStack.Top(0), 1);
    EXPECT_EQ(multiStack.Top(1), 10);
    EXPECT_EQ(multiStack.Top(2), 20);
}

TEST(TMultiStackTest, EqualityOperators)
{
    TMultiStack<int> multiStack1(30, 3);
    TMultiStack<int> multiStack2(30, 3);
    
    EXPECT_TRUE(multiStack1 == multiStack2);
    EXPECT_FALSE(multiStack1 != multiStack2);
    
    multiStack1.Push(0, 1);
    EXPECT_FALSE(multiStack1 == multiStack2);
    EXPECT_TRUE(multiStack1 != multiStack2);
    
    multiStack2.Push(0, 1);
    EXPECT_TRUE(multiStack1 == multiStack2);
    EXPECT_FALSE(multiStack1 != multiStack2);
    
    multiStack1.Push(1, 10);
    multiStack2.Push(1, 20);
    EXPECT_FALSE(multiStack1 == multiStack2);
    EXPECT_TRUE(multiStack1 != multiStack2);
}

TEST(TMultiStackTest, AccessOperator)
{
    TMultiStack<int> multiStack(30, 3);
    multiStack.Push(0, 1);
    multiStack.Push(0, 2);
    multiStack.Push(0, 3);
    
    TStack<int> stack = multiStack[0];
    EXPECT_EQ(stack.GetSize(), 3);
    EXPECT_EQ(stack.Top(), 3);
}

TEST(TMultiStackTest, AccessOperatorWithInvalidIndex)
{
    TMultiStack<int> multiStack(30, 3);
    ASSERT_ANY_THROW(multiStack[-1]);
    ASSERT_ANY_THROW(multiStack[3]);
}

TEST(TMultiStackTest, IteratorBasicFunctionality)
{
    TMultiStack<int> multiStack(30, 3);
    multiStack.Push(0, 1);
    multiStack.Push(0, 2);
    multiStack.Push(1, 10);
    multiStack.Push(2, 20);
    multiStack.Push(2, 21);
    
    auto it = multiStack.begin();
    EXPECT_NE(it, multiStack.end());
    
    std::vector<int> values;
    for (auto it = multiStack.begin(); it != multiStack.end(); ++it)
    {
        values.push_back(*it);
    }
    
    EXPECT_EQ(values.size(), 5);
    EXPECT_EQ(values[0], 1);
    EXPECT_EQ(values[1], 2);
    EXPECT_EQ(values[2], 10);
    EXPECT_EQ(values[3], 20);
    EXPECT_EQ(values[4], 21);
}

TEST(TMultiStackTest, IteratorOnEmptyMultiStack)
{
    TMultiStack<int> multiStack(30, 3);
    EXPECT_EQ(multiStack.begin(), multiStack.end());
}

TEST(TMultiStackTest, IteratorModification)
{
    TMultiStack<int> multiStack(30, 3);
    multiStack.Push(0, 1);
    multiStack.Push(1, 10);
    
    auto it = multiStack.begin();
    *it = 5;
    EXPECT_EQ(multiStack.Top(0), 5);
}

TEST(TMultiStackTest, DifferentDataTypes)
{
    TMultiStack<std::string> stringMultiStack(100, 2);
    stringMultiStack.Push(0, "hello");
    stringMultiStack.Push(0, "world");
    stringMultiStack.Push(1, "foo");
    stringMultiStack.Push(1, "bar");
    
    EXPECT_EQ(stringMultiStack.Top(0), "world");
    EXPECT_EQ(stringMultiStack.Top(1), "bar");
    
    TMultiStack<double> doubleMultiStack(100, 2);
    doubleMultiStack.Push(0, 3.14); 
    doubleMultiStack.Push(1, 2.71);
    
    EXPECT_DOUBLE_EQ(doubleMultiStack.Top(0), 3.14);
    EXPECT_DOUBLE_EQ(doubleMultiStack.Top(1), 2.71);
}

TEST(TMultiStackTest, LIFOBehaviorPerStack)
{
    TMultiStack<int> multiStack(30, 2);
    
    for (int i = 1; i <= 5; ++i)
    {
        multiStack.Push(0, i);
    }
    
    for (int i = 10; i <= 15; ++i)
    {
        multiStack.Push(1, i);
    }
    
    for (int i = 5; i >= 1; --i)
    {
        EXPECT_EQ(multiStack.Top(0), i);
        multiStack.Pop(0);
    }
    
    for (int i = 15; i >= 10; --i)
    {
        EXPECT_EQ(multiStack.Top(1), i);
        multiStack.Pop(1);
    }
    
    EXPECT_TRUE(multiStack.IsEmpty(0));
    EXPECT_TRUE(multiStack.IsEmpty(1));
}

TEST(TMultiStackTest, MemoryExpansion)
{
    TMultiStack<int> multiStack(6, 2);
    
    for (int i = 0; i < 6; ++i)
    {
        multiStack.Push(0, i);
    }
    
    EXPECT_EQ(multiStack.GetSize(0), 6);
    EXPECT_EQ(multiStack.GetTotalCapacity(), 6);
    
    ASSERT_ANY_THROW(multiStack.Push(0, 999));
    
    for (int i = 5; i >= 0; --i)
    {
        EXPECT_EQ(multiStack.Top(0), i);
        multiStack.Pop(0);
    }
}

TEST(TMultiStackTest, ComplexScenario)
{
    TMultiStack<int> multiStack(20, 4);
    
    multiStack.Push(0, 1);
    multiStack.Push(0, 2);
    multiStack.Push(1, 10);
    multiStack.Push(2, 20);
    multiStack.Push(2, 21);
    multiStack.Push(2, 22);
    multiStack.Push(3, 30);
    
    EXPECT_EQ(multiStack.GetSize(0), 2);
    EXPECT_EQ(multiStack.GetSize(1), 1);
    EXPECT_EQ(multiStack.GetSize(2), 3);
    EXPECT_EQ(multiStack.GetSize(3), 1);
    
    EXPECT_EQ(multiStack.Top(0), 2);
    EXPECT_EQ(multiStack.Top(1), 10);
    EXPECT_EQ(multiStack.Top(2), 22);
    EXPECT_EQ(multiStack.Top(3), 30);
    
    EXPECT_EQ(multiStack.Top(2), 22);
    multiStack.Pop(2);
    EXPECT_EQ(multiStack.Top(2), 21);
    multiStack.Pop(2);
    EXPECT_EQ(multiStack.GetSize(2), 1);
    EXPECT_EQ(multiStack.Top(2), 20);
    
    std::vector<int> allElements;
    for (auto it = multiStack.begin(); it != multiStack.end(); ++it)
    {
        allElements.push_back(*it);
    }
    
    EXPECT_GT(allElements.size(), 0);
}
