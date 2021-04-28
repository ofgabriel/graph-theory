#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <list>

#define BUILDING_DLL

#ifdef BUILDING_DLL
#define DLL_EXPORT_OR_IMPORT __declspec(dllexport);
#else
#define DLL_EXPORT_OR_IMPORT __declspec(dllimport)
#endif

using namespace std;

class Graph
{
public:
    Graph();
    
    virtual void clear();
    virtual void addVertex(int index) = 0;

    int graphSize_ = 0;
    int graphEdgesNumber_ = 0;

    bool loadGraphFromFilePath(string filePath);

    int getGraphSize();
    int getGraphEdgesNumber();
    int getGraphMinimumDegree();
    int getGraphMaximumDegree();
    int getGraphMeanDegree();
    int getGraphMedianDegree();
    int getGraphDiameter();
    int getDistance(int nodeId1, int nodeId2);

    virtual int getVerticeDegree(int nodeId) = 0;
    list<list<int> > getConnectedComponents();

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
    virtual void sortVertices() {}
    virtual vector<int> getNeighbors(int vertexIndex) = 0;
    virtual void addEdge(int vertex1, int vertex2);
    virtual void setupGraphWithSize(int graphSize);

    void setupGraphWithEdges(istream& file);

    void DFSUtil(int startNodeIndex, vector<int>& parent);
    int BFSUtil(int startNodeIndex, vector<int>& level, int goalIndex);
};