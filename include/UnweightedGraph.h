#pragma once

#include "Graph.h"
#include <vector>

class UnweightedGraph: public Graph
{
public:
    UnweightedGraph();
    bool loadGraphFromFilePath(string filePath) override;
    list<list<int> > getConnectedComponents();
    int getDistance(int nodeId1, int nodeId2);
    float getGraphDiameter() override;

    void breadthFirstSearch(
        int initialVertexIndex,
        vector<int>& parent,
        vector<int>& level
    );
    void depthFirstSearch(
        int initialVertexIndex,
        vector<int>& parent,
        vector<int>& level
    );
    
protected:
    virtual void addEdge(int vertex1, int vertex2);
    void DFSUtil(int startNodeIndex, vector<int>& parent);
    int BFSUtil(int startNodeIndex, vector<int>& level, int goalIndex);
    virtual vector<int> getNeighbors(int vertexIndex) = 0;
};
