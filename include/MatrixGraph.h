#pragma once

#include "UnweightedGraph.h"
#include <vector>

class MatrixGraph : public UnweightedGraph
{
public:
    MatrixGraph();
protected:
    void clear() override;
    void addVertex(int index) override;
    void setupGraphWithSize(int graphSize) override;
    void addEdge(int vertex1, int vertex2) override;
    vector<int> getNeighbors(int vertexIndex) override;
    int getVerticeDegree(int vertexIndex) override;
    
private:
    vector<vector<bool> > verticesMatrix_;
    vector<int> verticesDegrees_;
};