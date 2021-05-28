#include "Timing.h"
#include "Memory.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "WeightedGraph.h"
#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <random>
#include <fstream>
#include <iomanip>

using namespace std;

void timeBreadthFirstSearch(UnweightedGraph& graph, int iterations);
void timeDepthFirstSearch(UnweightedGraph& graph, int iterations);
int caseStudy(UnweightedGraph& graph);
int weightedGraphCaseStudy(WeightedGraph& graph);
int interactiveUsage(UnweightedGraph& graph);

int main(void) {
	string path = "./assets/grafo_W_1.txt";
    int executionMode;
	int dataStructure;
    int graphType;

	// cout << "Type the graph file path\n";
	// cin >> path;

    cout << "Pick an execution mode:\n";
	cout << "	1 - Case studies;\n";
	cout << "	2 - Interactive library;\n";
	cin >> executionMode;

    bool isWeightedGraph = Graph::isWeightedGraph(path);

    if (isWeightedGraph) {
	    WeightedGraph* graph;

        graph = new WeightedGraph();

        graph->loadGraphFromFilePath("./assets/grafo_W_1.txt");

        if (executionMode == 1) {
            return weightedGraphCaseStudy(*graph);
        }

        return 0;
    }

	cout << "Pick a structure for representing your graph:\n";
	cout << "	1 - Adjacency list;\n";
	cout << "	2 - Adjancecy matrix;\n";
	cin >> dataStructure;

	UnweightedGraph* graph;
	if (dataStructure == 1) {
		graph = new ListGraph();
	} else if (dataStructure == 2) {
		graph = new MatrixGraph();
	} else {
		cout << "Invalid choice";

		cin.get();

		return 0;
	}

    auto startMemory = getVirtualMemory();

    bool loadedFileSuccessfully = graph->loadGraphFromFilePath(path);

    auto memory = getVirtualMemory();
    cout << "Memory in KB " << memory - startMemory << "\n";

	if (!loadedFileSuccessfully) {
		cout << "File not found";

		cin.get();

		return 0;
	}

    if (executionMode == 1) {
        return caseStudy(*graph);
    } else if (executionMode == 2) {
        return interactiveUsage(*graph);
    } else {
        cout << "Invalid execution mode";

		cin.get();

        return 0;
    }

    cin.get();
    
    return 0;
}

void timeBreadthFirstSearch(UnweightedGraph& graph, int iterations)
{
    auto graphSize = graph.getGraphSize();
	INIT_TIMER();
	START_TIMER();

    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(1, graphSize + 1); // guaranteed unbiased

	for (int i = 0; i < iterations; i++)
	{
        auto startNode = uni(rng);
        vector<int> parent(graphSize, UINT_MAX);
        vector<int> level(graphSize);
        graph.breadthFirstSearch(startNode, parent, level);
	}
    
	STOP_TIMER();
    PRINT_TIMER("Timing BFS", iterations);
}

void timeDepthFirstSearch(UnweightedGraph& graph, int iterations)
{
    auto graphSize = graph.getGraphSize();
	INIT_TIMER();
	START_TIMER();

    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(1, graphSize + 1); // guaranteed unbiased

	for (int i = 0; i < iterations; i++)
	{
        auto startNode = uni(rng);
        vector<int> parent(graphSize, UINT_MAX);
        vector<int> level(graphSize);
        graph.depthFirstSearch(startNode, parent, level);
	}
    
	STOP_TIMER();
    PRINT_TIMER("Timing DFS", iterations);
}

int caseStudy(UnweightedGraph& graph)
{
    int initialVertices[3] = {1, 2, 3};
    int targetParents[3] = {10, 20, 30};

    INIT_TIMER();

    timeBreadthFirstSearch(graph, 1000);
    timeDepthFirstSearch(graph, 1000);

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
    unsigned int maxCounter = 0;
    unsigned int minCounter = UINT_MAX;

    cout << "Number of connected components: " << connectedComponents.size() << "\n";

    for (auto it: connectedComponents) {
        if (it.size() < minCounter) {
            minCounter = it.size();
        }

        if (it.size() > maxCounter) {
            maxCounter = it.size();
        }
    }

    cout << "Size of the biggest connected component: " << maxCounter << "\n";
    cout << "Size of the smallest connected component: " << minCounter << "\n";

    START_TIMER();
    auto diameter = graph.getGraphDiameter();
    cout << "Graph diameter: " << diameter << "\n";
    STOP_TIMER();
    PRINT_TIMER("Diameter calculated", 1);
    cout << "--------------------------\n\n";

    cin.get();

    return 0;
}

int interactiveUsage(UnweightedGraph& graph) {
    cout << "Writing results to file `graph_analysis.txt`...";

    ofstream output;

    output.open("graph_analysis.txt");

    list<list<int> > connectedComponents = graph.getConnectedComponents();

    output << "Number of vertices: " << graph.getGraphSize() << "\n";
    output << "Number of edges: " << graph.getGraphEdgesNumber() << "\n";
    output << "Minimum degree: " << graph.getGraphMinimumDegree() << "\n";
    output << "Maximum degree: " << graph.getGraphMaximumDegree() << "\n";
    output << setprecision(10) << "Mean degree: " << graph.getGraphMeanDegree() << "\n";
    output << "Median degree: " << graph.getGraphMedianDegree() << "\n";
    output << "Number of connected components: " << connectedComponents.size() << "\n";

    list<list<int> >::iterator it;

    for (auto l: connectedComponents) {
        output << "Connect component of size " << l.size() << "\n";
        for (auto cl: l) {
            output << cl << " ";
        }
        output << "\n";
    }

    output.close();

    cin.get();

	return 0;
}

int weightedGraphCaseStudy(WeightedGraph& graph)
{
    auto dist = graph.dijkstra(1, -1);

    cout << setprecision(10) << "Dist from 1 to 10: " << dist[10] << "\n";
    cout << setprecision(10) << "Dist from 1 to 20: " << dist[20] << "\n";
    cout << setprecision(10) << "Dist from 1 to 30: " << dist[30] << "\n";
    cout << setprecision(10) << "Dist from 1 to 40: " << dist[40] << "\n";
    cout << setprecision(10) << "Dist from 1 to 50: " << dist[50] << "\n";

    cout << setprecision(10) << "Eccentricity of vertex 10: " << graph.getEccentricity(10) << "\n";
    cout << setprecision(10) << "Eccentricity of vertex 20: " << graph.getEccentricity(20) << "\n";
    cout << setprecision(10) << "Eccentricity of vertex 30: " << graph.getEccentricity(30) << "\n";
    cout << setprecision(10) << "Eccentricity of vertex 40: " << graph.getEccentricity(40) << "\n";
    cout << setprecision(10) << "Eccentricity of vertex 50: " << graph.getEccentricity(50) << "\n";

    return 0;
}