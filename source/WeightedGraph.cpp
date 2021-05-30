#include "WeightedGraph.h"
#include <algorithm>
#include <fstream>
#include <limits>
#include "FibonacciQueue.h"
#include "Timing.h"

using namespace std;
float inf = numeric_limits<float>::max();

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
    auto distances = dijkstra(nodeId, -1);
    float maxDistance = -1;

    for (auto dist : distances) {
        if (dist > maxDistance) {
            maxDistance = dist;
        }
    }

    return maxDistance;
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
    int diameter = 0;

    INIT_TIMER();
    START_TIMER();
#pragma omp parallel for shared(diameter)
    for (int i = 1; i <= getGraphSize(); i++)
    {
        vector<int> level(getGraphSize(), -1);
        auto dist = dijkstra(i, -1);
        for (auto d : dist)
        {
            if (d > diameter && d != inf)
{
                diameter = d;
                STOP_TIMER();
                PRINT_TIMER("Found bigger diameter: " << diameter, 1);
            }
        }
    }
    return diameter;
}

int WeightedGraph::getVerticeDegree(int nodeId)
{
    return verticesList_[nodeId - 1].size();
}

vector<float> WeightedGraph::dijkstra(int initialVertex, int destVertex)
{
    FibonacciQueue<float, int> queue;
    vector<float> dist(getGraphSize(), inf);

    queue.push(0, initialVertex);
    dist[initialVertex - 1] = 0;

    while (!queue.empty()) {
        auto vertexId = queue.top();
        queue.pop();

        if (vertexId == destVertex)
        {
            return dist;
        }

        auto neighbors = getNeighbors(vertexId);
        for (auto neighborEdge : neighbors) {
            float neighborDist = dist[neighborEdge.neighbor - 1];
            if (neighborDist > dist[vertexId - 1] + neighborEdge.weight)
            {
                auto newWeight = dist[vertexId - 1] + neighborEdge.weight;
                if (neighborDist == inf)
                {
                    queue.push(newWeight, neighborEdge.neighbor);
                }
                else
                {
                    queue.decrease_key(neighborEdge.neighbor, newWeight);
                }
                dist[neighborEdge.neighbor - 1] = newWeight;
            }
        }
    }

    return dist;
}

vector<pair<int, Edge>> WeightedGraph::prim(int initialVertex, float* mstCost)
{
    auto mst = vector<pair<int, Edge>>(getGraphSize());

    const float inf = 10000000.0f;
    auto inMst = vector<bool>(getGraphSize());
    auto cost = vector<float>(getGraphSize(), inf);
    FibonacciQueue<float, int> queue;
    queue.push(0, initialVertex);

    while(!queue.empty())
    {
        auto edge = queue.topNode();
        int vertexId = edge->payload;
        float weight = edge->key;

        queue.pop();

        inMst[vertexId - 1] = true;
        
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
            mst[vertexId - 1] = {vertexId, newEdge};
		}        
    }
    
    if (mstCost != nullptr)
    {
        for (int i = 0; i < getGraphSize(); i++)
        {
            if (cost[i] != inf)
            {
                *mstCost += cost[i];
            }
        }
    }
    return mst;
}

vector<pair<int, Edge>> WeightedGraph::mst(int initialVertex, float* mstCost)
{
    return prim(initialVertex, mstCost);
}

void WeightedGraph::printGraph(ostream& output, vector<pair<int, Edge>>& graph)
{
    output << graph.size() << endl;
    for (auto edge : graph)
    {
        if (edge.first == 0)
        {
            continue;
        }
        output << edge.first << " " << edge.second.neighbor << " " << edge.second.weight << endl;
    }
}