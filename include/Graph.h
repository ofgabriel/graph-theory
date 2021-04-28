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
    vector<int> getVerticesDegrees();
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
    virtual int getNeighbor(int vertexIndex, int neighborId) = 0;
    virtual void addEdge(int vertex1, int vertex2);
    virtual void setupGraphWithSize(int graphSize);

    vector<int> verticesDegrees_;

    void setupGraphWithEdges(istream& file);

    void DFSUtil(int startNodeIndex, vector<int>& parent);
    int BFSUtil(int startNodeIndex, vector<int>& level, int goalIndex);
};