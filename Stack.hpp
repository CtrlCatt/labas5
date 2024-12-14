#pragma once
#include <memory>
#include <memory_resource>
#include <iostream>

template <typename T>
class Stack {
public:  
    struct Node {
        T value;
        Node* next; //указатель на след элемент узла
    };

private:
    Node* top_node;
    std::pmr::polymorphic_allocator<Node> allocator;

public:
    Stack(std::pmr::memory_resource* resource = std::pmr::get_default_resource())
        : top_node(nullptr), allocator(resource) {}

    ~Stack() {
        while (top_node) {
            pop();
        }
    }

    void push(const T& value) {
        Node* new_node = allocator.allocate(1); // указатель на зарезервированный ьлок памяти
        allocator.construct(new_node, Node{value, top_node});
        top_node = new_node;
    }

    void pop() {
        if (top_node) {
            Node* old_top = top_node;
            top_node = top_node->next;
            allocator.destroy(old_top);
            allocator.deallocate(old_top, 1);
        } else {
            std::cerr << "Stack is empty!\n";
        }
    }

    T& top() {
        if (top_node) {
            return top_node->value;
        }
        throw std::runtime_error("Stack is empty!");
    }

    bool empty() const {
        return top_node == nullptr;
    }

    // Итератор для стека
    class Iterator {
    private:
        Node* current;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit Iterator(Node* node) : current(node) {}

        T& operator*() const { return current->value; }

        Iterator& operator++() {
            current = current->next;
            return *this;
        }

        bool operator==(const Iterator& other) const { return current == other.current; }
        bool operator!=(const Iterator& other) const { return !(*this == other); }
    };

    Iterator begin() { return Iterator(top_node); }
    Iterator end() { return Iterator(nullptr); }
};
