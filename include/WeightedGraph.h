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
    vector<Edge> mst(int initialVertex, float* mstCost, ostream* output = nullptr);
    list<int> getPath(int destVertex, vector<int> prev);
    vector<float> dijkstra(int initialVertex, int destVertex, vector<int>*prev = nullptr);

    static void printGraph(ostream& output, vector<Edge>& graph, LabelProvider *labelProvider = nullptr);
    
protected:
    void addVertex(int index) override;
    void addEdge(int vertex1, int vertex2, float weight);
    void clear() override;
    void sortVertices() override;
    vector<Edge> getNeighbors(int vertexIndex);
    void setupGraphWithSize(int graphSize) override;

    vector<Edge> prim(int initialVertex, float* mstCost);
    vector<Edge> kruskal(float* mstCost);
    
private:
    vector<vector<int> > verticesList_;
    vector<Edge> edgesList_;
    class Subset
	{
	public:
		unsigned int parent;
		int rank;
	};

	int findSubset(Subset subsets[], int i);
	void subsetsUnion(Subset subsets[], int x, int y);
};
