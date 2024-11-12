#include "FixedBlockMemoryResource.hpp"
#include "Stack.hpp"
#include <iostream>
#include <string>

struct ComplexType {
    int id;
    std::string name;

    ComplexType(int id, std::string name) : id(id), name(std::move(name)) {}
};

void printMenu() {
    std::cout << "\nChoose an option:\n";
    std::cout << "1. Push integer to stack\n";
    std::cout << "2. Pop from stack\n";
    std::cout << "3. View top element\n";
    std::cout << "4. Check if stack is empty\n";
    std::cout << "5. Print all elements in stack\n";
    std::cout << "6. Push complex type to stack\n";
    std::cout << "7. Print all complex type elements\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter choice: ";
}

int main() {
    constexpr size_t block_size = sizeof(Stack<int>::Node);
    constexpr size_t block_count = 10;

    FixedBlockMemoryResource memory_resource(block_size, block_count);
    Stack<int> int_stack(&memory_resource);
    Stack<ComplexType> complex_stack(&memory_resource);

    int choice;
    bool running = true;

    while (running) {
        printMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int value;
                std::cout << "Enter integer value to push: ";
                std::cin >> value;
                int_stack.push(value);
                std::cout << "Pushed " << value << " to integer stack.\n";
                break;
            }
            case 2: {
                if (!int_stack.empty()) {
                    int_stack.pop();
                    std::cout << "Popped from integer stack.\n";
                } else {
                    std::cout << "Stack is empty! Cannot pop.\n";
                }
                break;
            }
            case 3: {
                if (!int_stack.empty()) {
                    std::cout << "Top element: " << int_stack.top() << "\n";
                } else {
                    std::cout << "Stack is empty!\n";
                }
                break;
            }
            case 4: {
                std::cout << (int_stack.empty() ? "Stack is empty.\n" : "Stack is not empty.\n");
                break;
            }
            case 5: {
                std::cout << "Integer Stack elements:\n";
                for (int value : int_stack) {
                    std::cout << value << ' ';
                }
                std::cout << "\n";
                break;
            }
            case 6: {
                int id;
                std::string name;
                std::cout << "Enter ID for complex type: ";
                std::cin >> id;
                std::cout << "Enter name for complex type: ";
                std::cin >> name;
                complex_stack.push(ComplexType(id, name));
                std::cout << "Pushed ComplexType (ID: " << id << ", Name: " << name << ") to complex stack.\n";
                break;
            }
            case 7: {
                std::cout << "Complex Stack elements:\n";
                for (auto& item : complex_stack) {
                    std::cout << "ID: " << item.id << ", Name: " << item.name << "\n";
                }
                break;
            }
            case 0: {
                running = false;
                std::cout << "Exiting program.\n";
                break;
            }
            default:
                std::cout << "Invalid choice! Please try again.\n";
                break;
        }
    }

    return 0;
}
