#pragma once

#include "Graph.h"
#include <vector>

class UnweightedGraph: public Graph
{
public:
    UnweightedGraph();
    bool loadGraphFromFilePath(string filePath) override;
    
protected:
    virtual void addEdge(int vertex1, int vertex2);
};
