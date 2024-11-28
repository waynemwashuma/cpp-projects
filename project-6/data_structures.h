#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <vector>
#include <iostream>
#include <iomanip>

enum COLOR
{
    WHITE,
    GREY
};

struct Edge
{
    unsigned int index;
    unsigned int u;
    unsigned int v;
    double w;
};

struct Vertex
{
    std::vector<Edge> edges;
    unsigned int index;
    COLOR color;
    double distance;
    Vertex *previous;
};

struct CheckPoint
{
    unsigned int point_index;
    double distance;
};

class Path
{
public:
    std::vector<CheckPoint>
        points;
    void print()
    {
        std::cout << "The shortest path from " << points[0].point_index + 1
                  << " to " << points[points.size() - 1].point_index + 1
                  << " is:" << std::endl;
        for (unsigned int i = 0; i < points.size(); i++)
        {
            std::cout << "[" << points[i].point_index + 1 << ":    "
                      << std::fixed << std::setprecision(2) << points[i].distance << "]";
            if (i != points.size() - 1)
                std::cout << "-->";
        }
        std::cout << "." << std::endl;
    };
    void print_length(int source, int destination)
    {
        float length = points[points.size() - 1].distance;
        std::cout << "The length of the shortest path from "
                  << source << " to " << destination << " is:     "
                  << std::fixed << std::setprecision(2) << length;
        std::cout << std::endl;
    };
};

#endif