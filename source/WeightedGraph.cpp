#include "WeightedGraph.h"
#include <algorithm>
#include <fstream>
#include "FibonacciQueue.h"

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

vector<float> WeightedGraph::dijkstra(int initialVertex, int destVertex)
{
    // if (initialVertex > getGraphSize()) {
    //     return vector<int>::empty;
    // }
    float inf = 10000000.0f;

    FibonacciQueue<float, int> queue;
    vector<float> dist(getGraphSize(), inf);

    queue.push(0, initialVertex);
    dist[initialVertex] = 0;

    while (!queue.empty()) {
        auto vertexId = queue.top();
        queue.pop();

        if (vertexId == destVertex)
        {
            return dist;
        }

        auto neighbors = getNeighbors(vertexId);
        for (auto neighborEdge : neighbors) {
            if (dist[neighborEdge.neighbor] > dist[vertexId] + neighborEdge.weight)
            {
                auto newWeight = dist[vertexId] + neighborEdge.weight;
                if (dist[neighborEdge.neighbor] == inf)
                {
                    queue.push(newWeight, neighborEdge.neighbor);
                }
                else
                {
                    queue.decrease_key(neighborEdge.neighbor, newWeight);
                }
                dist[neighborEdge.neighbor] = newWeight;
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
    FibonacciQueue<float, int> queue;
    queue.push(0, initialVertex);

    while(!queue.empty())
    {
        auto edge = queue.topNode();
        queue.pop();

        int vertexId = edge->payload;
        float weight = edge->key;

        inMst[vertexId - 1] = true;
        mstCost += weight;
        
        auto neighbors = getNeighbors(vertexId);
		for (auto neighborEdge : neighbors)
        {
            auto neighborId = neighborEdge.neighbor;
            if (inMst[neighborId - 1])
            {
                continue;
            }

            auto newEdge = Edge(neighborId, neighborEdge.weight);
            if (cost[neighborId - 1] == inf)
            {
                queue.push(newEdge.weight, newEdge.neighbor);
            }
            else if (cost[neighborId - 1] > neighborEdge.weight)
            {
                queue.decrease_key(neighborId, neighborEdge.weight);
            }

            cost[neighborId - 1] = neighborEdge.weight;
            mst[vertexId - 1] = make_pair(vertexId, newEdge);
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