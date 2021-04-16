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
    if (initialVertexIndex > getGraphSize()) {
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

void Graph::depthFirstSearch(
    int initialVertexIndex,
    vector<int>& parent,
    vector<int>& level
) {
    if (initialVertexIndex > getGraphSize()) {
        return;
    }

	vector<bool> explored = vector<bool>(getGraphSize(), false);
	stack<int> stack;

	stack.push(initialVertexIndex);

	parent[initialVertexIndex - 1] = 0;
	level[initialVertexIndex - 1] = 0;

	while (!stack.empty()) {
		int vertexId = stack.top();
		stack.pop();

		if (explored[vertexId - 1]) {
			continue;
		}

		explored[vertexId - 1] = true;
		for (int i = 0; i < getVerticesDegrees()[vertexId - 1]; i++) {
			int neighborId = getNeighbor(vertexId, i);

			if (parent[neighborId - 1] == UINT_MAX) {
				parent[neighborId - 1] = vertexId;
				level[neighborId - 1] = level[vertexId - 1] + 1;
			}

			stack.push(neighborId);
		}
	}
}

int Graph::getGraphSize() {
    return length_;
}

int Graph::getGraphMinimumDegree() {
    int minimumDegree = UINT_MAX;

    for (int i = 0; i < getGraphSize(); i++) {
        if (getVerticesDegrees()[i] < minimumDegree) {
            minimumDegree = getVerticesDegrees()[i];
        }
    }

    return minimumDegree;
}

int Graph::getGraphMaximumDegree() {
    int maximumDegree = UINT_MAX;

    for (int i = 0; i < getGraphSize(); i++) {
        if (getVerticesDegrees()[i] > maximumDegree) {
            maximumDegree = getVerticesDegrees()[i];
        }
    }

    return maximumDegree;
}

vector<int> Graph::getVerticesDegrees() {
    return verticesDegrees_;
}
