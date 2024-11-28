#include "graph.h"
#include "data_structures.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <optional>

#define MAX_DIST std::numeric_limits<double>::max()

unsigned int hash_key(unsigned int a, unsigned int b)
{
    return (a + b) * (a + b + 1) / 2 + a;
}

void Graph::addVertex()
{
    Vertex v;
    vertices.push_back(v);
}

void Graph::addEdge(unsigned int u, unsigned int v, double w, bool flag)
{
    Edge edge = Edge{0, u, v, w};
    if (u >= vertices.size() || v >= vertices.size())
    {
        return;
    }
    if (flag)
    {
        vertices[u].edges.push_back(edge);
    }
    else
    {
        vertices[u].edges.insert(vertices[u].edges.begin(), edge);
    }
}

void Graph::printAdjList() const
{
    for (unsigned int i = 0; i < vertices.size(); ++i)
    {
        std::vector<Edge> edges = vertices[i].edges;
        std::cout << "ADJ[" << i + 1 << "]:";
        for (unsigned int j = 0; j < edges.size(); j++)
        {
            Edge &edge = edges[j];
            std::cout << "-->[" << edge.u + 1 << " " << edge.v + 1 << ": "
                      << std::fixed << std::setprecision(2) << edge.w << "]";
        }
        std::cout << std::endl;
    }
}

void Graph::PrintPath(unsigned int from, unsigned int to)
{
    auto opt_path = computed_paths.find(hash_key(from, to));
    if (opt_path == computed_paths.end())
    {
        std::cout << "There is no path from " << from + 1 << " to " << to + 1 << "." << std::endl;
    }
    else
    {
        auto path = opt_path->second;
        path.print();
    }
}

void Graph::PrintLength(unsigned int from, unsigned int to)
{
    auto opt_path = computed_paths.find(hash_key(from, to));
    if (opt_path == computed_paths.end())
    {
        std::cout << "There is no path from " << from + 1 << " to "
                  << to + 1 << "." << std::endl;
    }
    else
    {
        auto path = opt_path->second;
        path.print_length(from + 1,to + 1);
    }
}

void Graph::SingleSource(unsigned int from)
{
    djikstras_algorithm(from);
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        auto path = build_path(vertices, i);
        if (std::get<0>(path))
        {
            computed_paths.insert(
                std::make_pair(
                    hash_key(from, i),
                    std::get<1>(path)));
        }
    }
}

void Graph::SinglePair(unsigned int from, unsigned int to)
{
    djikstras_algorithm(from);
    auto path = build_path(vertices, to);
    if (std::get<0>(path))
    {
        computed_paths.insert(
            std::make_pair(
                hash_key(from, to),
                std::get<1>(path)));
    }
}

void Graph::djikstras_algorithm(unsigned int source)
{
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        vertices[i].index = i;
        vertices[i].color = WHITE;
        vertices[i].distance = MAX_DIST;
        vertices[i].previous = NULL;
    }
    vertices[source].distance = 0;
    while (!all_visited(vertices))
    {
        Vertex &from = vertices[min_dist(vertices)];
        from.color = GREY;

        for (unsigned int i = 0; i < from.edges.size(); i++)
        {
            Edge &edge = from.edges[i];
            Vertex &to = vertices[edge.v];
            double alt = edge.w + from.distance;

            if (alt < to.distance)
            {
                to.distance = alt;
                to.previous = &from;
            }
        }
    }
}

unsigned int min_dist(std::vector<Vertex> vertices)
{
    int best_index = 0;
    float best_distance = std::numeric_limits<double>::max();

    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i].color == GREY)
            continue;
        if (vertices[i].distance < best_distance)
        {
            best_distance = vertices[i].distance;
            best_index = i;
        }
    }
    return best_index;
}

bool all_visited(std::vector<Vertex> vertices)
{
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i].color == WHITE)
            return false;
    }
    return true;
}

std::tuple<bool, Path> build_path(std::vector<Vertex> vertices, unsigned int destination)
{
    Path path;
    Vertex *tmp = &(vertices.at(destination));
    while (tmp != NULL)
    {
        if (tmp->distance == MAX_DIST)
            return std::make_tuple(false, path);
        ;
        path.points.insert(path.points.begin(), CheckPoint{tmp->index, tmp->distance});
        tmp = tmp->previous;
    }
    return std::make_tuple(true, path);
}