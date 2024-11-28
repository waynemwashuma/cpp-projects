#include "heap.h"
#include <iostream>
#include <algorithm>
#include <limits>

Heap::Heap(int limit) : limit(limit), size(0) {
    arr = new PVertex[limit];
}

Heap::~Heap() {
    delete[] arr;
}

void Heap::heapifyDown(int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < size && arr[left]->key < arr[smallest]->key) {
        smallest = left;
    }
    if (right < size && arr[right]->key < arr[smallest]->key) {
        smallest = right;
    }

    if (smallest != idx) {
        std::swap(arr[idx], arr[smallest]);
        heapifyDown(smallest);
    }
}

void Heap::heapifyUp(int idx) {
    int parent = (idx - 1) / 2;
    
    if (idx && arr[parent]->key > arr[idx]->key) {
        std::swap(arr[idx], arr[parent]);
        heapifyUp(parent);
    }
}

void Heap::insert(PVertex item) {
    if (size >= limit) {
        std::cerr << "Heap overflow\n";
        return;
    }
    
    arr[size] = item;
    heapifyUp(size);
    size++;
}

PVertex Heap::removeMin() {
    if (size <= 0) {
        std::cerr << "Heap underflow\n";
        return nullptr;
    }

    PVertex root = arr[0];
    arr[0] = arr[size - 1];
    size--;
    heapifyDown(0);

    return root;
}

void Heap::update(int idx, double newDist) {
    double oldDist = arr[idx]->key;
    arr[idx]->key = newDist;

    if (newDist < oldDist) {
        heapifyUp(idx);
    } else {
        heapifyDown(idx);
    }
}