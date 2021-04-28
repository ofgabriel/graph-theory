#include "MatrixGraph.h"

using namespace Lib;
using namespace std;

void MatrixGraph::addVertex(int index) {
    verticesMatrix_[index] = vector<bool>(getGraphSize());
}

void MatrixGraph::addEdge(int vertex1, int vertex2) {
    Graph::addEdge(vertex1, vertex2);

    verticesMatrix_[vertex1 - 1][vertex2 - 1] = true;
    verticesMatrix_[vertex2 - 1][vertex1 - 1] = true;
}

int MatrixGraph::getNeighbor(int vertexIndex, int neighborId) {
    int counter = neighborId;
    int i = 0;

    while (counter >= 0 && i < getGraphSize()) {
        if (verticesMatrix_[vertexIndex - 1][i]) {
            counter -= 1;
        }

        i++;
    }

    return i;
}

void MatrixGraph::setupGraphWithSize(int graphSize) {
    verticesMatrix_ = vector<vector<bool>>(graphSize);
    Graph::setupGraphWithSize(graphSize);
}