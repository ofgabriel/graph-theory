#include "MatrixGraph.h"

using namespace Lib;
using namespace std;

MatrixGraph::MatrixGraph() : Graph()
{
    verticesMatrix_ = vector<vector<bool>>();
    verticesDegrees_ = vector<int>();
}

void MatrixGraph::clear()
{
    verticesMatrix_.clear();
    verticesDegrees_.clear();
}

void MatrixGraph::addVertex(int index) {
    verticesMatrix_[index - 1] = vector<bool>(getGraphSize());
}

void MatrixGraph::addEdge(int vertex1, int vertex2) {
    Graph::addEdge(vertex1, vertex2);

    verticesMatrix_[vertex1 - 1][vertex2 - 1] = true;
    verticesMatrix_[vertex2 - 1][vertex1 - 1] = true;

    verticesDegrees_[vertex1 - 1] += 1;
    verticesDegrees_[vertex2 - 1] += 1;
}

int MatrixGraph::getNeighbor(int vertexIndex, int neighborId) {
    int i = 0;
    int counter = neighborId;
    int graphSize = getGraphSize();
    auto line = verticesMatrix_[vertexIndex - 1];

    while (counter >= 0 && i < graphSize) {
        if (line[i]) {
            counter--;
        }
        i++;
    }

    return i;
}

void MatrixGraph::setupGraphWithSize(int graphSize) {
    verticesMatrix_.resize(graphSize);
    verticesDegrees_.resize(graphSize);
    Graph::setupGraphWithSize(graphSize);
}

int MatrixGraph::getVerticeDegree(int vertexId) {
    return verticesDegrees_[vertexId - 1];
}