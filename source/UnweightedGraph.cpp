#include "UnweightedGraph.h"
#include <fstream>

using namespace std;

UnweightedGraph::UnweightedGraph()
{
}


bool UnweightedGraph::loadGraphFromFilePath(string filePath) {
    clear();
    ifstream file;
    int graphSize = 0;
    file.open(filePath);
    file >> graphSize;

    if (!file.good()) {
        return false;
    }

    setupGraphWithSize(graphSize);
    int i = 1;
    while (i <= graphSize_)
    {
        addVertex(i);
        i++;
    }

    int vertex1, vertex2;
    while (file >> vertex1 >> vertex2) {
        addEdge(vertex1, vertex2);
    }

    sortVertices();
    return true;
}

void UnweightedGraph::addEdge(int vertex1, int vertex2) {
    graphEdgesNumber_++;
}