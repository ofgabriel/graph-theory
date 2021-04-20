#include "Graph.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <climits>
#include <stack>
#include <random>

using namespace Lib;
using namespace std;

void Graph::loadGraphFromFilePath(string filePath) {
    ifstream file;

    file.open(filePath);
    file >> graphSize_;

    setupGraphWithSize(graphSize_);
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
    return graphSize_;
}

int Graph::getGraphEdgesNumber() {
    return graphEdgesNumber_;
}

int Graph::getGraphMinimumDegree() {
    int minimumDegree = UINT_MAX;

    for (int i = 0; i < getGraphSize(); i++) {
        if (getVerticesDegrees()[i] < minimumDegree) {
            minimumDegree = getVerticesDegrees()[i];
        }
    }

void Graph::DFSUtil(int initialVertexIndex, vector<int>& parent)
{
	stack<int> stack;

	stack.push(initialVertexIndex);

	parent[initialVertexIndex - 1] = 0;

	while (!stack.empty()) {
		int nodeId = stack.top();
		stack.pop();
		
		for (int i = 0; i < getVerticesDegrees()[nodeId - 1]; i++) {
			int neighborId = getNeighbor(nodeId, i);

			if (parent[neighborId - 1] != UINT_MAX) {
				continue;
			}

			parent[neighborId - 1] = initialVertexIndex;
			stack.push(neighborId);
		}
	}
}

int Graph::BFSUtil(int initialVertexIndex, vector<int>& level, int goalIndex)
{
	int diameter = 0;
	queue<int> queue;

	queue.push(initialVertexIndex);
	level[initialVertexIndex - 1] = 0;

	while (!queue.empty()) {
		int nodeId = queue.front();
		queue.pop();

		if (goalIndex == nodeId) {
			return diameter;
		}
		
		for (int i = 0; i < getVerticesDegrees()[nodeId - 1]; i++) {
			int neighborId = getNeighbor(nodeId, i);

			if (level[neighborId - 1] != -1) {
				continue;
			}

			level[neighborId - 1] = level[nodeId - 1] + 1;

            if ((int)level[neighborId - 1] > diameter) {
				diameter = level[neighborId - 1];
			}

			queue.push(neighborId);
		}
	}
	return diameter;
}
