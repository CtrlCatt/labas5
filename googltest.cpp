#include <gtest/gtest.h>
#include "FixedBlockMemoryResource.hpp"
#include "Stack.hpp"

// Тест FixedBlockMemoryResource
TEST(FixedBlockMemoryResourceTest, BasicAllocationAndDeallocation) {
    constexpr size_t block_size = sizeof(Stack<int>::Node);
    constexpr size_t block_count = 10;

    FixedBlockMemoryResource memory_resource(block_size, block_count);

    // Проверка базовой аллокации
    void* ptr1 = memory_resource.allocate(block_size, alignof(Stack<int>::Node));
    EXPECT_NE(ptr1, nullptr);

    void* ptr2 = memory_resource.allocate(block_size, alignof(Stack<int>::Node));
    EXPECT_NE(ptr2, nullptr);

    // Проверка деалокации
    memory_resource.deallocate(ptr1, block_size, alignof(Stack<int>::Node));
    memory_resource.deallocate(ptr2, block_size, alignof(Stack<int>::Node));
}

// Тест стека
TEST(StackTest, PushPopAndTop) {
    constexpr size_t block_size = sizeof(Stack<int>::Node);
    constexpr size_t block_count = 10;

    FixedBlockMemoryResource memory_resource(block_size, block_count);
    Stack<int> stack(&memory_resource);

    // Проверка push
    stack.push(10);
    stack.push(20);
    stack.push(30);

    // Проверка top
    EXPECT_EQ(stack.top(), 30);

    // Проверка pop
    stack.pop();
    EXPECT_EQ(stack.top(), 20);

    stack.pop();
    EXPECT_EQ(stack.top(), 10);

    stack.pop();
    EXPECT_TRUE(stack.empty());
}

// Тест итератора
TEST(StackTest, Iterator) {
    constexpr size_t block_size = sizeof(Stack<int>::Node);
    constexpr size_t block_count = 10;

    FixedBlockMemoryResource memory_resource(block_size, block_count);
    Stack<int> stack(&memory_resource);

    stack.push(1);
    stack.push(2);
    stack.push(3);

    std::vector<int> expected = {3, 2, 1};
    size_t index = 0;

    for (int value : stack) {
        EXPECT_EQ(value, expected[index++]);
    }
}

// Главная функция для запуска тестов
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}