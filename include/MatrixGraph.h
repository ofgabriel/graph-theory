#pragma once

#include "UnweightedGraph.h"
#include <vector>

class MatrixGraph : public UnweightedGraph
{
public:
    MatrixGraph();

protected:
    void addVertex(int index) override;
    void clear() override;
    int getVerticeDegree(int vertexIndex) override;
    void addEdge(int vertex1, int vertex2) override;
    vector<int> getNeighbors(int vertexIndex) override;
    void setupGraphWithSize(int graphSize) override;
    
private:
    vector<vector<bool> > verticesMatrix_;
    vector<int> verticesDegrees_;
};
