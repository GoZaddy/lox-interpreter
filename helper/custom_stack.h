#ifndef CUSTOM_STACK_H
#define CUSTOM_STACK_H

#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T>
class Stack {
private:
    std::vector<T> elements;

public:
    // Adds an element to the top of the stack
    void push(const T& value);

    // Removes the element from the top of the stack and returns it
    T pop();

    // Returns true if the stack is empty, false otherwise
    bool empty() const;

    // Returns the element at the specified index (0-based)
    T at(size_t index) const;

    // Returns the element at the top of the stack
    T peek() const;

    // Returns the size of the stack
    size_t size() const;
};


#include "custom_stack.tpp"


#endif