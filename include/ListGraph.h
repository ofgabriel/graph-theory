#pragma once

#include "Graph.h"
#include <forward_list>
#include <vector>

class ListGraph: public Graph
{
public:
    ListGraph();
    
protected:
    void addVertex(int index) override;
    void clear() override;
    int getVerticeDegree(int vertexIndex) override;
    void sortVertices() override;
    void addEdge(int vertex1, int vertex2) override;
    vector<int> getNeighbors(int vertexIndex) override;
    void setupGraphWithSize(int graphSize) override;
private:
    vector<vector<int> > verticesList_;
};
