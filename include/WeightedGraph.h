#pragma once

#include "Graph.h"
#include "Edge.h"
#include <vector>

class WeightedGraph: public Graph
{
public:
    WeightedGraph();
    bool loadGraphFromFilePath(string filePath) override;
    float getEccentricity(int nodeId);
    
    float getGraphDiameter() override;
    int getVerticeDegree(int nodeId) override;
    float mst(int initialVertex, vector<pair<int, Edge>>& mst);

    vector<float> dijkstra(int initialVertex, int destVertex);

protected:
    void addVertex(int index) override;
    void addEdge(int vertex1, int vertex2, float weight);
    void clear() override;
    void sortVertices() override;
    vector<Edge> getNeighbors(int vertexIndex);
    void setupGraphWithSize(int graphSize) override;

    float prim(int initialVertex, vector<pair<int, Edge>>& mst);
    
private:
    vector<vector<Edge>> verticesList_;
};
