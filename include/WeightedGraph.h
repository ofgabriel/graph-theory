#pragma once

#include "Graph.h"
#include "Edge.h"
#include <vector>

class WeightedGraph: public Graph
{
public:
    WeightedGraph();
    bool loadGraphFromFilePath(string filePath) override;
    
protected:
    void addEdge(int source, int destination, float weight);
    void clear() override;

private:
    vector<vector<Edge>> verticesList_;
};
