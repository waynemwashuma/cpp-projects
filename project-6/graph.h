
#ifndef GRAPH_H
#define GRAPH_H

#include "data_structures.h"
#include <vector>
#include <map>

class Graph {
public:
    std::vector<Vertex> vertices;
    std::map<unsigned int,Path> computed_paths;

public:
    void addVertex();
    void addEdge(unsigned int u, unsigned int v, double w,bool flag);
    void printAdjList() const;
    void PrintPath(unsigned int from,unsigned int to);
    void PrintLength(unsigned int from,unsigned int to);
    void SingleSource(unsigned int from);
    void SinglePair(unsigned int from,unsigned int to);
    void djikstras_algorithm(unsigned int source);
};

bool all_visited(std::vector<Vertex> vertices);
unsigned int min_dist(std::vector<Vertex> vertices);
std::tuple<bool,Path> build_path(std::vector<Vertex> vertices, unsigned int destination);
#endif