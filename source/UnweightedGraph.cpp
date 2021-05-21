#include "UnweightedGraph.h"
#include "Timing.h"
#include <fstream>
#include <queue>
#include <stack>

using namespace std;

UnweightedGraph::UnweightedGraph()
{
}


bool UnweightedGraph::loadGraphFromFilePath(string filePath) {
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

void UnweightedGraph::addEdge(int vertex1, int vertex2) {
    graphEdgesNumber_++;
}

void UnweightedGraph::DFSUtil(int initialVertexIndex, vector<int>& parent)
{
	stack<int> stack;

	stack.push(initialVertexIndex);

	parent[initialVertexIndex - 1] = 0;

	while (!stack.empty()) {
		int nodeId = stack.top();
		stack.pop();
		
        auto neighbors = getNeighbors(nodeId);
		for (auto neighborId : neighbors) {

			if (parent[neighborId - 1] != -1) {
				continue;
			}

			parent[neighborId - 1] = initialVertexIndex;
			stack.push(neighborId);
		}
	}
}

int UnweightedGraph::BFSUtil(int initialVertexIndex, vector<int>& level, int goalIndex)
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


list<list<int> > UnweightedGraph::getConnectedComponents() {
    vector<list<int>*> map = vector<list<int>*>(getGraphSize());
    list<list<int> > connectedComponents = list<list<int> >();

    vector<int> parent(getGraphSize(), -1);

    for (int vertexId = 1; vertexId < getGraphSize() + 1; vertexId++) {
        if (parent[vertexId - 1] != -1) {
            continue;
        }

        DFSUtil(vertexId, parent);

        connectedComponents.push_back(list<int>());

        map[vertexId - 1] = &connectedComponents.back();
        map[vertexId - 1]->push_back(vertexId);
    }

    for (int vertexId = 0; vertexId < getGraphSize(); vertexId++) {
        if (parent[vertexId] == -1) {
            continue;
        }

        if (parent[vertexId] == 0) {
            continue;
        }

        map[parent[vertexId] - 1]->push_back(vertexId + 1);
    }

    connectedComponents.sort([](const list<int> component1, const list<int> component2) {
        return component1.size() > component2.size();
    });

    return connectedComponents;
}

void UnweightedGraph::breadthFirstSearch(
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

            if (parent[neighborId - 1] != -1) {
                continue;
            }

            parent[neighborId - 1] = vertexId;
            level[neighborId - 1] = level[vertexId - 1] + 1;
            
            discoveredVertices.push(neighborId);
        }
    }
}

void UnweightedGraph::depthFirstSearch(
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
			if (parent[neighborId - 1] == -1) {
				parent[neighborId - 1] = vertexId;
				level[neighborId - 1] = level[vertexId - 1] + 1;
			}

			stack.push(neighborId);
		}
	}
}


float UnweightedGraph::getGraphDiameter() {
    int diameter = 0;
    INIT_TIMER();
    START_TIMER();
#pragma omp parallel for shared(diameter)
    for (int i = 1; i <= getGraphSize(); i++) {
        vector<int> level(getGraphSize(), -1);
        int d = BFSUtil(i, level, -1);

        if (d > diameter) {
            diameter = d;
            STOP_TIMER();
            PRINT_TIMER("Found bigger diameter: " << diameter, 1);
        }
    }
    return diameter;
}

int UnweightedGraph::getDistance(int nodeId1, int nodeId2) {
    vector<int> graphLevels10(getGraphSize(), -1);
    BFSUtil(nodeId1, graphLevels10, nodeId2);
    return graphLevels10[nodeId2 - 1];
}