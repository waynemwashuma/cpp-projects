#ifndef HEAP_H
#define HEAP_H

#include "data_structures.h"

class Heap {
private:
    int limit;
    int size;
    PVertex* arr;

    void heapifyDown(int idx);
    void heapifyUp(int idx);

public:
    Heap(int limit);
    ~Heap();

    void insert(PVertex item);
    void update(int idx, double newDist);
    PVertex removeMin();
};

#endif
