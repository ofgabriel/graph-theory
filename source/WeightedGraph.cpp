#include "WeightedGraph.h"
#include <algorithm>
#include <fstream>

using namespace std;

WeightedGraph::WeightedGraph() : Graph()
{
    verticesList_ = vector<vector<Edge>>();
}

void WeightedGraph::clear()
{
    verticesList_.clear();
}

bool WeightedGraph::loadGraphFromFilePath(string filePath) {
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

    int sourceNode, destinationNode;
    float edgeWeight;
    while (file >> sourceNode >> destinationNode >> edgeWeight) {
        addEdge(sourceNode, destinationNode, edgeWeight);
    }

    sortVertices();
    return true;
}

void WeightedGraph::addEdge(int source, int destination, float weight) {
    graphEdgesNumber_++;

}