#pragma once

#include "Graph.h"
#include <vector>

namespace Lib
{
    class MatrixGraph : public Graph
    {
    public:
        void addVertex(int index) override;
        void addEdge(int vertex1, int vertex2) override;

        int getNeighbor(int vertexIndex, int neighborId) override;
    protected:
        void setupGraphWithSize(int graphSize) override;
    private:
        vector<vector<bool> > verticesMatrix_;
    };
}
