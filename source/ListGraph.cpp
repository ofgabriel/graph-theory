#include "ListGraph.h"
#include <algorithm>

using namespace std;

ListGraph::ListGraph() : UnweightedGraph()
{
    verticesList_ = vector<vector<int>>();
}

void ListGraph::clear()
{
    verticesList_.clear();
}

void ListGraph::addVertex(int index) {
    verticesList_[index - 1] = vector<int>();
}

void ListGraph::addEdge(int vertex1, int vertex2) {
    UnweightedGraph::addEdge(vertex1, vertex2);

    verticesList_[vertex1 - 1].push_back(vertex2);
    verticesList_[vertex2 - 1].push_back(vertex1);
}

void ListGraph::sortVertices() {
    for (int i = 0; i < getGraphSize(); i++) {
        sort(verticesList_[i].begin(), verticesList_[i].end());
    }
}

vector<int> ListGraph::getNeighbors(int vertexIndex) {
    return verticesList_[vertexIndex - 1];
}

void ListGraph::setupGraphWithSize(int graphSize) {
    Graph::setupGraphWithSize(graphSize);
    verticesList_.resize(graphSize);
}

int ListGraph::getVerticeDegree(int vertexId) {
    return verticesList_[vertexId - 1].size();
}