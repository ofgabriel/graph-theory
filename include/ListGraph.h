#pragma once

#include "Graph.h"
#include <forward_list>
#include <vector>

class ListGraph: public Graph
{
public:
    ListGraph();

    void addVertex(int index) override;
    void clear() override;
    
    void addEdge(int vertex1, int vertex2) override;
    void sortVertices() override;

    int getNeighbor(int vertexIndex, int neighborId) override;
protected:
    void setupGraphWithSize(int graphSize) override;
private:
    vector<vector<int> > verticesList_;
};
