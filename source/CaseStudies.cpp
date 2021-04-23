#include "../source/ListGraph.h"
#include <iostream>
#include <vector>
#include <string>

const int SOURCES_NUMBER = 6;

using namespace std;

int main(void) {
    ListGraph graph;

    string graphSources[SOURCES_NUMBER] = {
        "./assets/grafo_1.txt",
        "./assets/grafo_2.txt",
        "./assets/grafo_3.txt",
        "./assets/grafo_4.txt",
        "./assets/grafo_5.txt",
        "./assets/grafo_6.txt"
    };
    int initialVertices[3] = {1, 2, 3};
    int targetParents[3] = {10, 20, 30};

    for (int i = 0; i < SOURCES_NUMBER; i++) {
        graph.loadGraphFromFilePath(graphSources[i]);

        for (int j = 0; j < 3; j++) {
            vector<int> graphBfsParents(graph.getGraphSize(), UINT_MAX);
            vector<int> graphDfsParents(graph.getGraphSize(), UINT_MAX);
            vector<int> graphBfsLevels(graph.getGraphSize());
            vector<int> graphDfsLevels(graph.getGraphSize());

            graph.breadthFirstSearch(initialVertices[j], graphBfsParents, graphBfsLevels);
            graph.depthFirstSearch(initialVertices[j], graphDfsParents, graphDfsLevels);

            for (int k = 0; k < 3; k++) {
                cout << "Parent of vertex " << targetParents[k] << " on BFS spanning tree (starting at vertex ";
                cout << initialVertices[j] << "): " << graphBfsParents[targetParents[k] - 1] << "\n";

                cout << "Parent of vertex " << targetParents[k] << " on DFS spanning tree (starting at vertex ";
                cout << initialVertices[j] << "): " << graphDfsParents[targetParents[k] - 1] << "\n";
            }
        }
    }

    return 0;
}
