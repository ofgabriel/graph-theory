#include <vector>
#include <iostream>

using namespace std;

namespace Lib
{
    class Graph
    {
    protected:
        vector<int> verticesDegrees_;

        virtual void addVertex(int index) = 0;
        virtual void setupGraphWithEdges(istream& file) = 0;
        virtual int getNeighbor(int vertexIndex, int neighborId) = 0;

        void setupGraphWithSize(int graphSize);
    public:
        int graphSize_;
        int graphEdgesNumber_;

        void loadGraphFromFilePath(string filePath);

        int getGraphSize();
        int getGraphEdgesNumber();
        int getGraphMinimumDegree();
        int getGraphMaximumDegree();
        int getGraphMeanDegree();
        int getGraphMedianDegree();
        vector<int> getVerticesDegrees();

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
    };
}
