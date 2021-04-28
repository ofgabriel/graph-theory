#include "../source/ListGraph.h"
#include <iostream>
#include <vector>
#include <string>
#include <climits>

const int SOURCES_NUMBER = 2;

using namespace std;

int main(void) {
    ListGraph graph;

    string graphSources[SOURCES_NUMBER] = {
        "./assets/grafo_1.txt",
        "./assets/grafo_2.txt"
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

        vector<int> graphParents10(graph.getGraphSize(), UINT_MAX);
        vector<int> graphLevels10(graph.getGraphSize());
        vector<int> graphParents20(graph.getGraphSize(), UINT_MAX);
        vector<int> graphLevels20(graph.getGraphSize());

        graph.breadthFirstSearch(10, graphParents10, graphLevels10);
        graph.breadthFirstSearch(20, graphParents20, graphLevels20);

        cout << "Distance between vertices 10 and 20: " << graphLevels10[19] << "\n";
        cout << "Distance between vertices 10 and 30: " << graphLevels10[29] << "\n";
        cout << "Distance between vertices 20 and 30: " << graphLevels20[29] << "\n";

        list<list<int> > connectedComponents = graph.getConnectedComponents();
        list<list<int> >::iterator it;
        int maxCounter = 0;
        int minCounter = UINT_MAX;

        cout << "Number of connected components: " << connectedComponents.size() << "\n";

        for (it = connectedComponents.begin(); it != connectedComponents.end(); it++) {
            if (it->size() < minCounter) {
                minCounter = it->size();
            }

            if (it->size() > maxCounter) {
                maxCounter = it->size();
            }
        }

        cout << "Size of the biggest connected component: " << maxCounter << "\n";
        cout << "Size of the smallest connected component: " << minCounter << "\n";

        cout << "Graph diameter: " << graph.getGraphDiameter() << "\n";
    }

    return 0;
}
