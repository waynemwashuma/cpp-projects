#include "stack.h"
#include <iostream>

Stack::Stack(int max) : max(max), top(-1) {
    items = new PVertex[max];
}

Stack::~Stack() {
    delete[] items;
}

bool Stack::isEmpty() const {
    return top == -1;
}

void Stack::push(PVertex item) {
    if (top < max - 1) {
        items[++top] = item;
    } else {
        std::cerr << "Stack overflow\n";
    }
}

PVertex Stack::pop() {
    if (!isEmpty()) {
        return items[top--];
    }
    std::cerr << "Stack underflow\n";
    return nullptr;
}

