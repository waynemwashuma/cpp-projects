#ifndef STACK_H
#define STACK_H

#include "data_structures.h"

class Stack {
private:
    int max;
    int top;
    PVertex* items;

public:
    Stack(int max);
    ~Stack();
    bool isEmpty() const;
    void push(PVertex item);
    PVertex pop();
};

#endif
