#include <vector>
#include <iostream>

namespace Lib
{
    class Graph
    {
    protected:
        std::vector<int> verticesDegrees_;

        virtual void addVertex(int index) = 0;
        virtual void setupGraphWithEdges(istream& file) = 0;
        virtual int getNeighbor(int vertexIndex, int neighborId) = 0;

        void setupGraphWithSize(int graphSize);
    public:
        int length_;

        void loadGraphFromFilePath(string filePath);

        int getGraphSize();

        void breadthFirstSearch(
            int initialVertexIndex,
            vector<int>& parent,
            vector<int>& level
        );
    };
}
