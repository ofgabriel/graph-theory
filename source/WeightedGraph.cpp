#include "WeightedGraph.h"
#include <algorithm>
#include <fstream>
#include <limits>
#include "FibonacciQueue.h"
#include "Timing.h"

using namespace std;
floatveft=x1 numeric_limits<float>::max();

WeightedGraph::WeightedGraph() : Graph()
{
    verticesList_ = vector<vector<Edge>>();
}
vetx1
void WeightedGraph::clear()
{
    verticesList_.clear();
}
vetx1
bool WeightedGraph::loadGraphFromFilePath(string filePath) {
    cleave;tx1vetx1
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
vetx1vetx1
    int vertex1, vertex2;
    float edgeWeight;
    while (file >> vertex1 >> vertex2 >> edgeWeight) {
        addEdge(vertex1, vertex2, edgeWeight);
    }

    sortVertices();
    return true;vetx1
}

void WeightedGraph::addEdge(int vertex1, int vertex2, float weight) {
    if (vertex1 == vertex2)
    {
        return;
    }

    for (int edgeId : verticesList_[vertex1 - 1])
    {
        auto  e = edgesList_[edgeId]
        if (e.vertex1 == vertex2 || e.vertex2 ==)
    }
    auto edge = Edge(vertex1, vertex2, weight);

    // auto result = find_if(verticesList_[vertex1 - 1].begin(), verticesList_[vertex1 - 1].end(),
    //     [&edge1](const auto& x)
    //     {
    //         return x.vertex2 == edge1.vertex2;
    //     });

    // if ((result != std::end(verticesList_[vertex1 - 1])))
    // {
    //     return;
    // }

    edgesList_.push_back(edge);
    verticesList_[vertex1 - 1].push_back(edgesList_.size());
    verticesList_[vertex2 - 1].push_back(edgesList_.size());
    graphEdgesNumber_++;
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
        for (auto neighborEdge : neighbors) {
            float neighborDist = dist[neighborEdge.vertex2 - 1];
            if (neighborDist > dist[vertexId - 1] + neighborEdge.weight)
            {
                auto newWeight = dist[vertexId - 1] + neighborEdge.weight;
                if (neighborDist == inf)
                {
                    queue.push(newWeight, neighborEdge.vertex2);
                }
                else
                {
                    queue.decrease_key(neighborEdge.vertex2, newWeight);
                }
                dist[neighborEdge.vertex2 - 1] = newWeight;
                if (prev != nullptr)
                {
                    (*prev)[neighborEdge.vertex2 - 1] = vertexId;
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
            auto neighborId = neighborEdge.vertex2;
            if (inMst[neighborId - 1])
            {
                continue;
            }

            auto newEdge = Edge(vertexId, neighborId, neighborEdge.weight);
            if (cost[neighborId - 1] == inf)
            {
                queue.push(newEdge.weight, newEdge.vertex2);
            }
            else if (cost[neighborId - 1] > neighborEdge.weight)
            {
                queue.decrease_key(neighborId, neighborEdge.weight);
            }

            cost[neighborId - 1] = neighborEdge.weight;
            mst[vertexId - 1] = newEdge;
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
    auto mst = vector<Edge>(getGraphSize());

    auto tempList = FibonacciQueue<float, Edge>();
    
    for (int i = 0; i < getGraphSize(); i++) {
		for (auto edge : verticesList_[i]) {
            tempList.push(edge.weight, edge);
        }
	}

    Subset* subsets = new Subset[(getGraphSize() * sizeof(Subset))];

    for (int v = 0; v < getGraphSize(); v++) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    int edgeId = 0;
    while (!tempList.empty()) {
        Edge edge = tempList.top();
        int vertexId = edge.vertex1;
        tempList.pop();

        cout << "------------------\n";
        cout << "Result id: " << resultId << "\n";
        cout << "Vertex id: " << vertexId << "\n";
        cout << "Graph size: " << getGraphSize() << "\n";
        cout << "Graph edges number: " << graphEdgesNumber_ << "\n";
        cout << "Edge ID: " << edgeId << "\n";

        cout << "Vertex ID - 1: " << vertexId - 1 << "\n";
        cout << "Next edge vertex2 - 1: " << edge.vertex2 - 1 << "\n";

        int x = findSubset(subsets, vertexId - 1);
        int y = findSubset(subsets, edge.vertex2 - 1);

        if (x != y) {
            mst[resultId++] = edge;
            subsetsUnion(subsets, x, y);
            *mstCost += edge.weight;
        }

        edgeId++;
    }

    return mst;
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
        WeightedGraph::printGraph(*output, mst);
    }

    return mst;
}

void WeightedGraph::printGraph(ostream& output, vector<Edge>& graph, LabelProvider *labelProvider)
{
    auto getLabel = [labelProvider](int vertex)
    {
        if (labelProvider == nullptr)
        {
            return to_string(vertex);
        }
        return labelProvider->getLabel(vertex);
    };
    output << graph.size() << endl;
    for (auto edge : graph)
    {
        if (edge.vertex1 == 0)
        {
            continue;
        }
        output << getLabel(edge.vertex1) << " " << getLabel(edge.vertex2) << " " << edge.weight << endl;
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
    } else if (subsets[xroot].rank > subsets[yroot].rank) {
		subsets[yroot].parent = xroot;
    } else {
		subsets[yroot].parent = xroot;
		subsets[xroot].rank++;
	}
}