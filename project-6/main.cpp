#include "graph.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        showError("Usage: ./PJ3 <InputFile> <GraphType> <Flag>");
        return 1;
    }

    std::string graphType = argv[2];
    bool flag = (bool)(std::stoi(argv[3]));

    if ((graphType != "DirectedGraph" && graphType != "UndirectedGraph") || (flag != 0 && flag != 1)) {
        showError("Usage: ./PJ3 <InputFile> <GraphType> <Flag>");
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        showError("Unable to open input file " + std::string(argv[1]));
        return 1;
    }

    int numVertices, numEdges;
    inputFile >> numVertices >> numEdges;

    Graph graph;

    for (int i = 0; i < numVertices; i++)
    {
        graph.addVertex();
    }
    
    for (int i = 0; i < numEdges; ++i) {
        unsigned int edgeId, u, v;
        double w;
        inputFile >> edgeId >> u >> v >> w;
        graph.addEdge((u - 1), (v - 1), w,flag);
        if (graphType == "UndirectedGraph") {
            graph.addEdge((v - 1), (u - 1), w,flag);
        }
    }

    inputFile.close();

    std::string instruction;
    while (std::getline(std::cin, instruction)) {
        unsigned int source,destination; 
        std::istringstream iss(instruction);
        std::string command;
        iss >> command;

        if (command == "Stop") {
            break;
        } else if (command == "PrintADJ") {
            graph.printAdjList();
        }else if (command == "PrintLength") {
            iss >> source >> destination;
            graph.PrintLength((source - 1),(destination - 1));
        } else if (command == "PrintPath") {
            iss >> source >> destination;
            graph.PrintPath((source - 1),(destination - 1));
        } else if (command == "SinglePair") {
            iss >> source >> destination;
            graph.SinglePair((source - 1),(destination - 1));
        }else if (command == "SingleSource") {
            iss >> source;
            graph.SingleSource((source - 1));
        }
        else {
            showError("Invalid instruction.");
        }
    }

    return 0;
}