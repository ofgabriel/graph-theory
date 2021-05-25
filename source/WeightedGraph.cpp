#include "WeightedGraph.h"
#include <algorithm>
#include <fstream>
#include <queue>

using namespace std;

WeightedGraph::WeightedGraph() : Graph()
{
    verticesList_ = vector<vector<Edge>>();
}

void WeightedGraph::clear()
{
    verticesList_.clear();
}

bool WeightedGraph::loadGraphFromFilePath(string filePath) {
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
    float edgeWeight;
    while (file >> vertex1 >> vertex2 >> edgeWeight) {
        addEdge(vertex1, vertex2, edgeWeight);
    }

    sortVertices();
    return true;
}

void WeightedGraph::addEdge(int vertex1, int vertex2, float weight) {
    graphEdgesNumber_++;

    verticesList_[vertex1 - 1].push_back(Edge(vertex2, weight));
    verticesList_[vertex2 - 1].push_back(Edge(vertex1, weight));
}

void WeightedGraph::sortVertices() {
    for (int i = 0; i < getGraphSize(); i++) {
        sort(verticesList_[i].begin(), verticesList_[i].end());
    }
}

float WeightedGraph::getEccentricity(int nodeId)
{
    return 0;
}

vector<Edge> WeightedGraph::getNeighbors(int vertexIndex)
{
    return verticesList_[vertexIndex - 1];
}

void WeightedGraph::addVertex(int index)
{

}

void WeightedGraph::setupGraphWithSize(int graphSize)
{
    Graph::setupGraphWithSize(graphSize);
    verticesList_.resize(graphSize);
}


float WeightedGraph::getGraphDiameter()
{
}

int WeightedGraph::getVerticeDegree(int nodeId)
{

}

vector<int> WeightedGraph::dijkstra(int initialVertex, int destVertex = -1)
{
    // if (initialVertex > getGraphSize()) {
    //     return vector<int>::empty;
    // }
    float inf = 10000000.0f;

    priority_queue<Edge, vector<Edge>, greater<Edge>> queue;
    vector<int> dist(getGraphSize(), inf);

    queue.push(Edge(initialVertex, 0));
    dist[initialVertex] = 0;

    while (!queue.empty()) {
        auto vertexId = queue.top().neighbor;
        queue.pop();

        if (vertexId == destVertex)
        {
            return dist;
        }

        auto neighbors = getNeighbors(vertexId);
        for (auto neighborEdge : neighbors) {
            if (dist[neighborEdge.neighbor] > dist[vertexId] + neighborEdge.weight) {
                dist[neighborEdge.neighbor] = dist[vertexId] + neighborEdge.weight;
                queue.push(Edge(neighborEdge.neighbor, dist[vertexId]));
            }
        }
    }

    return dist;
}

float WeightedGraph::prim(int initialVertex, vector<pair<int, Edge>>& mst)
{
    float mstCost = 0;
    const float inf = 10000000.0f;
    auto inMst = vector<bool>(getGraphSize());
    auto cost = vector<double>(getGraphSize(), inf);
    priority_queue<Edge, vector<Edge>, greater<Edge>> queue;
    queue.push(Edge(initialVertex, 0));

    while(!queue.empty())
    {
        auto edge = queue.top();
        queue.pop();

        inMst[edge.neighbor - 1] = true;
        mstCost += edge.weight;
        
        auto neighbors = getNeighbors(edge.neighbor);
		for (auto neighborEdge : neighbors)
        {
            auto neighborId = neighborEdge.neighbor;
            if (!inMst[neighborId - 1] && cost[neighborId - 1] > neighborEdge.weight)
            {
                auto newEdge = Edge(neighborId, neighborEdge.weight);
                cost[neighborId - 1] = neighborEdge.weight;
                mst[edge.neighbor - 1] = make_pair(edge.neighbor, newEdge);
                queue.push(newEdge);
            }
		}        
    }
    
    for (int i = 0; i < getGraphSize(); i++)
	{
		if (cost[i] != inf)
		{
			mstCost += cost[i];
		}
	}
    return mstCost;
}

float WeightedGraph::mst(int initialVertex, vector<pair<int, Edge>>& result)
{
    return prim(initialVertex, result);
}