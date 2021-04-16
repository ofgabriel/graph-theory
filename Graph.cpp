#include "Graph.h"

#include <fstream>
#include <queue>

using namespace Lib;
using namespace std;

void Graph::loadGraphFromFilePath(string filePath) {
    ifstream file;

    file.open(filePath);
    file >> length_;

    setupGraphWithSize(length_);
    setupGraphWithEdges(file);
}

void Graph::setupGraphWithSize(int graphSize) {
    verticesDegrees_ = vector<int>(graphSize);
    int i = 0;

    while (i < getGraphSize()) {
        addVertex(i);
        i++;
    }

    return;
}

void Graph::breadthFirstSearch(
    int initialVertexIndex,
    vector<int>& parent,
    vector<int>& level
) {
    if (initialVertexIndex > length_) {
        return;
    }

    queue<int> discoveredVertices;

    discoveredVertices.push(initialVertexIndex);
    // What's that used for?
    parent[initialVertexIndex - 1] = 0;
    level[initialVertexIndex - 1] = 0;

    while (!discoveredVertices.empty()) {
        int vertexId = discoveredVertices.front();
        discoveredVertices.pop();

        for (int i = 0; i < verticesDegrees_[vertexId - 1]; i++) {
            int neighborId = getNeighbor(vertexId, i);

            if (level[neighborId - 1] != UINT_MAX) {
                continue;
            }

            parent[neighborId - 1] = vertexId;
            level[neighborId - 1] = level[vertexId - 1] + 1;
            
            discoveredVertices.push(neighborId);
        }
    }
}

int Graph::getGraphSize() {
    return length_;
}
