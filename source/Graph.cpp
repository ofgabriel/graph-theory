#include "Graph.h"
#include "Timing.h"
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

int Graph::getGraphSize() {
    return graphSize_;
}

int Graph::getGraphEdgesNumber() {
    return graphEdgesNumber_;
}

int Graph::getGraphMinimumDegree() {
    int minimumDegree = INT_MAX;

    for (int i = 1; i <= getGraphSize(); i++) {
        auto deg = getVerticeDegree(i);
        if (deg < minimumDegree) {
            minimumDegree = deg;
        }
    }

    return minimumDegree;
}

int Graph::getGraphMaximumDegree() {
    int maximumDegree = 0;

    for (int i = 1; i <= getGraphSize(); i++) {
        auto deg = getVerticeDegree(i);
        if (deg > maximumDegree) {
            maximumDegree = deg;
        }
    }

    return maximumDegree;
}

float Graph::getGraphMeanDegree() {
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

void Graph::setupGraphWithSize(int graphSize) {
    graphSize_ = graphSize;
}

void Graph::printPath(ostream& output, vector<int> path, LabelProvider *labelProvider)
{
    auto getLabel = [labelProvider](int vertex)
    {
        if (labelProvider == nullptr)
        {
            return to_string(vertex);
        }
        return labelProvider->getLabel(vertex);
    };
    
    for (auto vertex : path)
    {
        output << getLabel(vertex) << ", "; 
    }
    output << endl;
}