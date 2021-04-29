#include "MatrixGraph.h"

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
    verticesMatrix_[index - 1] = vector<bool>(getGraphSize(), false);
    verticesDegrees_[index - 1] = 0;
}

void MatrixGraph::addEdge(int vertex1, int vertex2) {
    Graph::addEdge(vertex1, vertex2);

    verticesDegrees_[vertex1 - 1] += 1;
    verticesDegrees_[vertex2 - 1] += 1;

    verticesMatrix_[vertex1 - 1][vertex2 - 1] = true;
    verticesMatrix_[vertex2 - 1][vertex1 - 1] = true;
}

vector<int> MatrixGraph::getNeighbors(int vertexIndex) {
    auto neighbors = vector<int>();
    auto line = verticesMatrix_[vertexIndex - 1];
    int count = 0;
    int deg = getVerticeDegree(vertexIndex);

    int i = 0;
    int graphSize = getGraphSize();
    std::find(line.begin(), line.end(), true);
    while (count < deg && i < graphSize) {
        if (line[i]) {
            neighbors.push_back(i + 1);
            count++;
        }
        i++;
    }
    return neighbors;
}

void MatrixGraph::setupGraphWithSize(int graphSize) {
    Graph::setupGraphWithSize(graphSize);
    verticesMatrix_.resize(graphSize);
    verticesDegrees_.resize(graphSize, 0);
}

int MatrixGraph::getVerticeDegree(int vertexId) {
    return verticesDegrees_[vertexId - 1];
}