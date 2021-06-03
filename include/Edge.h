#pragma once

#include <functional>

using namespace std;

struct Edge
{
public:
	int vertex1;
	int vertex2;
	float weight;

	Edge()
	{
		this->vertex1 = -1;
		this->vertex2 = -1;
		this->weight = -1;
	}

	Edge(int vertex1, int vertex2, float weight)
	{
		this->vertex1 = vertex1;
		this->vertex2 = vertex2;
		this->weight = weight;
	}

	int getNeighbor(int vertex)
	{
		return vertex1 == vertex ? vertex2 : vertex1;
	}

	bool operator<(const Edge &other) const
	{
		return (vertex2 < other.vertex2);
	}

	bool operator==(const Edge &other) const
	{
		return (vertex2 == other.vertex2 && vertex1 == other.vertex1);
	}
};
