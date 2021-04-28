#include "Timing.h"
#include "Memory.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <random>

const int SOURCES_NUMBER = 1;

using namespace std;
using namespace Lib;

void timeBreadthFirstSearch(Graph& graph, int iterations);
void timeDepthFirstSearch(Graph& graph, int iterations);

int main(void) {
    auto startMemory = getVirtualMemory();
    ListGraph graph;

    string graphSources[SOURCES_NUMBER] = {
        "./assets/grafo_5.txt",
        //"./assets/grafo_2.txt",
        //"./assets/grafo_3.txt",
        //"./assets/grafo_4.txt",
        //"./assets/grafo_5.txt",
        //"./assets/grafo_6.txt"
    };
    int initialVertices[3] = {1, 2, 3};
    int targetParents[3] = {10, 20, 30};

    INIT_TIMER();
    for (int i = 0; i < SOURCES_NUMBER; i++) {
        cout << "Loading graph " << graphSources[i] << "\n";

        START_TIMER();
        graph.loadGraphFromFilePath(graphSources[i]);
        STOP_TIMER();
        PRINT_TIMER("File read", 1);

        auto memory = getVirtualMemory();
        cout << "Memory in KB " << memory - startMemory << "\n";

        timeBreadthFirstSearch(graph, 1000);
        timeDepthFirstSearch(graph, 1000);

        /*
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
        //*/

        /*
        START_TIMER();
        cout << "Graph diameter: " << graph.getGraphDiameter() << "\n";
        STOP_TIMER();
        PRINT_TIMER("Diameter calculated", 1);
        //*/
        cout << "--------------------------\n\n";
    }

    return 0;
}

void timeBreadthFirstSearch(Graph& graph, int iterations)
{
    auto graphSize = graph.getGraphSize();
	INIT_TIMER();
	START_TIMER();

    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(1, graphSize + 1); // guaranteed unbiased

//#pragma omp parallel for
	for (int i = 0; i < iterations; i++)
	{
		//cout << i << "/" << iterations;
        auto startNode = uni(rng);
        vector<int> parent(graphSize, UINT_MAX);
        vector<int> level(graphSize);
        graph.breadthFirstSearch(startNode, parent, level);
		//printf("\r");
	}
    
	STOP_TIMER();
    PRINT_TIMER("Timing BFS", iterations);
}

void timeDepthFirstSearch(Graph& graph, int iterations)
{
    auto graphSize = graph.getGraphSize();
	INIT_TIMER();
	START_TIMER();

    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(1, graphSize + 1); // guaranteed unbiased

//#pragma omp parallel for
	for (int i = 0; i < iterations; i++)
	{
		//cout << i << "/" << iterations;
        auto startNode = uni(rng);
        vector<int> parent(graphSize, UINT_MAX);
        vector<int> level(graphSize);
        graph.depthFirstSearch(startNode, parent, level);
		//printf("\r");
	}
    
	STOP_TIMER();
    PRINT_TIMER("Timing DFS", iterations);
}