#pragma once
#include <memory_resource>
#include <vector>
#include <iostream>

class FixedBlockMemoryResource : public std::pmr::memory_resource {
private:
    std::vector<void*> free_blocks;
    std::pmr::memory_resource* upstream;  // Аллокатор для резервного выделения памяти
    size_t block_size;  // Размер блока памяти
    size_t block_count;  // Количество блоков в пуле
    void* pool_start;  // Начало пула памяти

public:
    FixedBlockMemoryResource(size_t block_size, size_t block_count)
        : block_size(block_size), block_count(block_count) {
        // Выделяем большой блок памяти для пула
        pool_start = ::operator new(block_size * block_count); // указатель на начало блока памяти
        upstream = std::pmr::get_default_resource();
        
        // Разделяем блок на фиксированные части и добавляем их в свободные блоки
        for (size_t i = 0; i < block_count; ++i) {
            free_blocks.push_back(static_cast<char*>(pool_start) + i * block_size);
        }
    }

    ~FixedBlockMemoryResource() {
        ::operator delete(pool_start);  // Освобождаем память пула
    }

protected:
    void* do_allocate(size_t bytes, size_t alignment) override {
        if (bytes > block_size) {
            return upstream->allocate(bytes, alignment);  // Если размер превышает блок, используем верхний аллокатор
        }

        if (!free_blocks.empty()) {
            void* ptr = free_blocks.back();
            free_blocks.pop_back();
            return ptr;
        }
        throw std::bad_alloc();
    }

    void do_deallocate(void* ptr, size_t bytes, size_t alignment) override {
        if (bytes <= block_size) {
            free_blocks.push_back(ptr);
        } else {
            upstream->deallocate(ptr, bytes, alignment);
        }
    }

    bool do_is_equal(const memory_resource& other) const noexcept override {
        return this == &other;
    }
};

