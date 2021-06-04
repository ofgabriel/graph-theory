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
    verticesList_ = vector<vector<int>>();
    edgesList_ = vector<Edge>();
}

void WeightedGraph::clear()
{
    verticesList_.clear();
    edgesList_.clear();
}

bool WeightedGraph::loadGraphFromFilePath(string filePath)
{
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

    sort(edgesList_.begin(), edgesList_.end(), compareEdge);
    buildVerticesList();

    return true;
}

bool WeightedGraph::compareEdge(Edge a, Edge b)
{
    return (a.weight < b.weight);
}

void WeightedGraph::addEdge(int vertex1, int vertex2, float weight) {
    if (vertex1 == vertex2)
    {
        return;
    }

    for (int edgeId : verticesList_[vertex1 - 1])
    {
        auto  e = edgesList_[edgeId];
        if (e.vertex1 == vertex2)
        {
            return;
        }
    }
    auto edge = Edge(vertex1, vertex2, weight);

    edgesList_.push_back(edge);
    verticesList_[vertex1 - 1].push_back(graphEdgesNumber_);
    verticesList_[vertex2 - 1].push_back(graphEdgesNumber_);
    graphEdgesNumber_++;
}

void WeightedGraph::buildVerticesList()
{
    int edgeIndex = 0;
    for (int i = 0; i < getGraphSize(); i++)
    {
        verticesList_[i].clear();
    }
    for (auto edge : edgesList_) {
        this->verticesList_[edge.vertex1 - 1].push_back(edgeIndex);
        this->verticesList_[edge.vertex2 - 1].push_back(edgeIndex);
        edgeIndex++;
    }
}

void WeightedGraph::sortVertices()
{
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

vector<int> WeightedGraph::getNeighbors(int vertexIndex)
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

vector<float> WeightedGraph::dijkstra(int initialVertex, int destVertex, vector<int>*prev)
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
        for (auto neighborEdgeId : neighbors) {
            auto neighborEdge = edgesList_[neighborEdgeId];
            auto neighborId = neighborEdge.getNeighbor(vertexId);

            float neighborDist = dist[neighborId - 1];
            if (neighborDist > dist[vertexId - 1] + neighborEdge.weight)
            {
                auto newWeight = dist[vertexId - 1] + neighborEdge.weight;
                if (neighborDist == inf)
                {
                    queue.push(newWeight, neighborId);
                }
                else
                {
                    queue.decrease_key(neighborId, newWeight);
                }
                dist[neighborId - 1] = newWeight;
                if (prev != nullptr)
                {
                    (*prev)[neighborId - 1] = vertexId;
                }
            }
        }
    }
    return dist;
}

list<int> WeightedGraph::getPath(int destVertex, vector<int> prev)
{
    list<int> path;
    int nodeId = destVertex;
    while (nodeId != 0)
    {
        path.push_front(nodeId);
        nodeId = prev[nodeId - 1];
    }
    return path;
}

vector<Edge> WeightedGraph::prim(int initialVertex, float* mstCost)
{
    auto mst = vector<Edge>(getGraphSize());

    auto inMst = vector<bool>(getGraphSize());
    auto cost = vector<float>(getGraphSize(), inf);
    FibonacciQueue<float, int> queue;
    queue.push(0, initialVertex);

    while(!queue.empty())
    {
        int vertexId = queue.top();
        queue.pop();

        inMst[vertexId - 1] = true;
        
        auto neighbors = getNeighbors(vertexId);
		for (auto neighborEdgeId : neighbors)
        {
            auto neighborEdge = edgesList_[neighborEdgeId];
            auto neighborId = neighborEdge.getNeighbor(vertexId);

            if (inMst[neighborId - 1] == false && cost[neighborId - 1] > neighborEdge.weight)
            {
                auto newEdge = Edge(vertexId, neighborId, neighborEdge.weight);
                if (cost[neighborId - 1] == inf)
                {
                    queue.push(newEdge.weight, neighborId);
                }
                else
                {
                    queue.decrease_key(neighborId, neighborEdge.weight);
                }
                cost[neighborId - 1] = neighborEdge.weight;
                mst[vertexId - 1] = newEdge;
            }
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

vector<Edge> WeightedGraph::kruskal(float* mstCost)
{
	int resultId = 0;
    auto forest = vector<Edge>(getGraphSize());
    Subset* subsets = new Subset[forest.size()];

    for (int v = 0; v < getGraphSize(); v++) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    int edgeId = 0;
    for (auto edge : edgesList_) {
        int vertexId = edge.vertex1;

        int x = findSubset(subsets, vertexId - 1);
        int y = findSubset(subsets, edge.getNeighbor(vertexId) - 1);

        if (x != y) {
            forest[resultId++] = edge;
            subsetsUnion(subsets, x, y);
            *mstCost += edge.weight;
        }

        edgeId++;
    }

    return forest;
}

vector<Edge> WeightedGraph::mst(int initialVertex, float* mstCost, ostream* output)
{
    vector<Edge> mst;

    if (initialVertex == 0) {
        mst = kruskal(mstCost);
    } else {
        mst = prim(initialVertex, mstCost);
    }

    if (output != nullptr)
    {
        int mstCount = 0;
        (*output) << "Peso total da MST: " << (*mstCost) << endl; 
        for (int i = 0; i < mst.size(); i++)
        {
            if (mst[i].vertex1 != -1)
            {
                mstCount++;
            }
        }
        WeightedGraph::printGraph(*output, mstCount, mst);
    }

    return mst;
}

void WeightedGraph::printGraph(ostream& output, int graphSize, vector<Edge>& graph)
{
    output << graphSize << endl;
    for (auto edge : graph)
    {
        if (edge.vertex1 == -1)
        {
            continue;
        }
        output << edge.vertex1 << " " << edge.vertex2 << " " << edge.weight << endl;
    }
}

int WeightedGraph::findSubset(Subset subsets[], int i)
{
	if (subsets[i].parent != i) {
		subsets[i].parent = findSubset(subsets, subsets[i].parent);
    }

	return subsets[i].parent;
}

void WeightedGraph::subsetsUnion(Subset subsets[], int x, int y)
{
	int xroot = findSubset(subsets, x);
	int yroot = findSubset(subsets, y);

	if (subsets[xroot].rank < subsets[yroot].rank) {
		subsets[xroot].parent = yroot;
    } else {
		subsets[yroot].parent = xroot;
    }
    
    if (subsets[xroot].rank == subsets[yroot].rank) {
		subsets[yroot].rank++;
	}
}