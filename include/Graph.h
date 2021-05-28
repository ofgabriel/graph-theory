#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include "LabelProvider.h"

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

    static bool isWeightedGraph(string filePath);
    
    virtual bool loadGraphFromFilePath(string filePath) = 0;

    int getGraphSize();
    int getGraphEdgesNumber();
    int getGraphMinimumDegree();
    int getGraphMaximumDegree();
    float getGraphMeanDegree();
    int getGraphMedianDegree();

    virtual float getGraphDiameter() = 0;
    virtual int getVerticeDegree(int nodeId) = 0;
    void printPath(ostream& output, vector<int> path, LabelProvider *labelProvider);

protected:
    int graphSize_ = 0;
    int graphEdgesNumber_ = 0;

    virtual void addVertex(int index) = 0;
    virtual void clear();
    virtual void sortVertices() {}
    virtual void setupGraphWithSize(int graphSize);
};
