#include "Graph.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <climits>
#include <stack>

using namespace std;

Graph::Graph()
{
}

void Graph::clear()
{
    graphSize_ = 0;
    graphEdgesNumber_ = 0;
}

bool Graph::loadGraphFromFilePath(string filePath) {
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

int Graph::getGraphSize() {
    return graphSize_;
}

int Graph::getGraphEdgesNumber() {
    return graphEdgesNumber_;
}

int Graph::getGraphMinimumDegree() {
    int minimumDegree = UINT_MAX;

    for (int i = 1; i <= getGraphSize(); i++) {
        auto deg = getVerticeDegree(i);
        if (deg < minimumDegree) {
            minimumDegree = deg;
        }
    }

    return minimumDegree;
}

int Graph::getGraphMaximumDegree() {
    int maximumDegree = UINT_MAX;

    for (int i = 1; i <= getGraphSize(); i++) {
        auto deg = getVerticeDegree(i);
        if (deg > maximumDegree) {
            maximumDegree = deg;
        }
    }

    return maximumDegree;
}

int Graph::getGraphMeanDegree() {
    if (getGraphSize() == 0) {
        return 0;
    }

    return (float)getGraphEdgesNumber() / (float)getGraphSize();
}

int Graph::getGraphMedianDegree() {
    int size = getGraphSize();

    if (size == 0) {
        return 0;
    }

    vector<int> degrees(size);
    for (int i = 1; i <= getGraphSize(); i++) {
        degrees[i - 1] = getVerticeDegree(i);
    }

    if (size % 2 == 0) {
        return (degrees[size / 2 - 1] + degrees[size / 2]) / 2;
    }

    return degrees[size / 2];
}

int Graph::getGraphDiameter() {
    int diameter = 0;
#pragma omp parallel for shared(diameter)
    for (int i = 1; i <= getGraphSize(); i++) {
        vector<int> level(getGraphSize(), -1);
        int d = BFSUtil(i, level, UINT_MAX);

        if (d > diameter) {
            diameter = d;
            cout << "Found bigger diameter: " << diameter << "\n";
        }
    }
    return diameter;
}

int Graph::getDistance(int nodeId1, int nodeId2) {
    vector<int> graphLevels10(getGraphSize(), -1);
    BFSUtil(nodeId1, graphLevels10, nodeId2);
    return graphLevels10[nodeId2 - 1];
}

list<list<int> > Graph::getConnectedComponents() {
    vector<list<int>*> map = vector<list<int>*>(getGraphSize());
    list<list<int> > connectedComponents = list<list<int> >();

    vector<int> parent(getGraphSize(), UINT_MAX);

    for (int vertexId = 1; vertexId < getGraphSize() + 1; vertexId++) {
        if (parent[vertexId - 1] != UINT_MAX) {
            continue;
        }

        DFSUtil(vertexId, parent);

        connectedComponents.push_front(list<int>());

        map[vertexId - 1] = &*connectedComponents.begin();
        map[vertexId - 1]->push_front(vertexId);
    }

    for (int vertexId = 0; vertexId < getGraphSize(); vertexId++) {
        if (parent[vertexId] == UINT_MAX) {
            continue;
        }

        if (parent[vertexId] == 0) {
            continue;
        }

        map[parent[vertexId] - 1]->push_front(vertexId + 1);
    }

    return connectedComponents;
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

    parent[initialVertexIndex - 1] = 0;
    level[initialVertexIndex - 1] = 0;

    while (!discoveredVertices.empty()) {
        int vertexId = discoveredVertices.front();
        discoveredVertices.pop();

        auto neighbors = getNeighbors(vertexId);
        for (auto neighborId : neighbors) {

            if (parent[neighborId - 1] != UINT_MAX) {
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
        auto neighbors = getNeighbors(vertexId);
		for (auto neighborId : neighbors) {
			if (parent[neighborId - 1] == UINT_MAX) {
				parent[neighborId - 1] = vertexId;
				level[neighborId - 1] = level[vertexId - 1] + 1;
			}

			stack.push(neighborId);
		}
	}
}

void Graph::addEdge(int vertex1, int vertex2) {
    graphEdgesNumber_++;
}

void Graph::setupGraphWithSize(int graphSize) {
    graphSize_ = graphSize;
}

void Graph::DFSUtil(int initialVertexIndex, vector<int>& parent)
{
	stack<int> stack;

	stack.push(initialVertexIndex);

	parent[initialVertexIndex - 1] = 0;

	while (!stack.empty()) {
		int nodeId = stack.top();
		stack.pop();
		
        auto neighbors = getNeighbors(nodeId);
		for (auto neighborId : neighbors) {

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
		
        auto neighbors = getNeighbors(nodeId);
		for (auto neighborId : neighbors) {

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
