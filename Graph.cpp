#include "Graph.h"
#include <fstream>

using namespace Lib;
using namespace std;

void Graph::loadGraphFromFilePath(string filePath) {
    ifstream file;

    file.open(filePath);
    file >> length_;

    setupGraphWithSize(length_);
    setupGraphWithEdges(file);
}

void Graph::setupGraphWithSize(int graphSize) {
    verticesDegrees_ = vector<int>(graphSize);
    int i = 0;

    while (i < getGraphSize()) {
        addVertex(i);
        i++;
    }

    return;
}

int Graph::getGraphSize() {
    return length_;
}
