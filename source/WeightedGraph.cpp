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

    int vertex1, vertex2;
    float edgeWeight;
    while (file >> vertex1 >> vertex2 >> edgeWeight) {
        addEdge(vertex1, vertex2, edgeWeight);
    }

    sortVertices();
    return true;
}

void WeightedGraph::addEdge(int vertex1, int vertex2, float weight) {
    graphEdgesNumber_++;

    verticesList_[vertex1 - 1].push_back(Edge(vertex2, weight));
    verticesList_[vertex2 - 1].push_back(Edge(vertex1, weight));
}

void WeightedGraph::sortVertices() {
    for (int i = 0; i < getGraphSize(); i++) {
        sort(verticesList_[i].begin(), verticesList_[i].end());
    }
}

float WeightedGraph::getEccentricity(int nodeId)
{
    return 0;
}

vector<Edge> WeightedGraph::getNeighbors(int vertexIndex)
{

}

void WeightedGraph::addVertex(int index)
{

}

void WeightedGraph::setupGraphWithSize(int graphSize)
{

}


float WeightedGraph::getGraphDiameter()
{
}

int WeightedGraph::getVerticeDegree(int nodeId)
{

}