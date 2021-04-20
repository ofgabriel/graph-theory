#include "Graph.h"
#include "Timing.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <climits>
#include <stack>
#include <random>

using namespace std;

bool Graph::loadGraphFromFilePath(string filePath) {
    INIT_TIMER;
    START_TIMER;

    ifstream file;

    file.open(filePath);
    file >> graphSize_;

    if (!file.good()) {
        return false;
    }

    setupGraphWithSize(graphSize_);
    setupGraphWithEdges(file);

    sortVertices();

    STOP_TIMER();
    PRINT_TIMER("File read", 1);

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

int Graph::getGraphMeanDegree() {
    if (getGraphSize() == 0) {
        return 0;
    }

    return (float)getGraphEdgesNumber() / (float)getGraphSize();
}

int Graph::getGraphMedianDegree() {
    int size = getVerticesDegrees().size();

    if (size == 0) {
        return 0;
    }

    vector<int> degrees(getVerticesDegrees());

    if (size % 2 == 0) {
        return (degrees[size / 2 - 1] + degrees[size / 2]) / 2;
    }

    return degrees[size / 2];
}

int Graph::getGraphDiameter() {
    int diameter = 0;
    vector<int> order = vector<int>(getGraphSize());

    for (int i = 0; i < getGraphSize(); i++) {
        order[i] = i;
    }

    default_random_engine random;
    shuffle(order.begin(), order.end(), random);

#pragma omp parallel for shared(diameter)
    for (int i = 0; i < getGraphSize(); i++) {
        int vertexId = order[i] = 1;
        vector<int> level(getGraphSize(), -1);
        int d = BFSUtil(vertexId, level, UINT_MAX);

        if (d > diameter) {
            diameter = d;
            cout << diameter << "\n";
        }
    }

    return diameter;
}

vector<int> Graph::getVerticesDegrees() {
    return verticesDegrees_;
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

void Graph::addEdge(int vertex1, int vertex2) {
    graphEdgesNumber_++;

    verticesDegrees_[vertex1 - 1] += 1;
    verticesDegrees_[vertex2 - 1] += 1;
}

void Graph::setupGraphWithEdges(istream& file) {
    int vertex1, vertex2;

    while (file >> vertex1 >> vertex2) {
        addEdge(vertex1, vertex2);
    }
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
