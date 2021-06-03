#pragma once

#include <functional>

using namespace std;

struct Edge
{
	int source;
	int vertex2;
	float weight;

	Edge()
	{
		this->source = -1;
		this->vertex2 = -1;
		this->weight = -1;
	}

	Edge(int source, int vertex2, float weight)
	{
		this->source = source;
		this->vertex2 = vertex2;
		this->weight = weight;
	}

	bool operator<(const Edge &other) const
	{
		return (vertex2 < other.vertex2);
	}

	// bool operator > (const Edge& other) const
	// {
	//     return (weight > other.weight);
	// }

	bool operator==(const Edge &other) const
	{
		return (vertex2 == other.vertex2 && source == other.source);
	}

	std::size_t operator()(const Edge& e) const
	{
		auto hashFunction = std::hash<int>();
		const int multiplier = 17;
		std::size_t hashcode = 1;
		hashcode = multiplier * hashcode + hashFunction(e.source);
		hashcode = multiplier * hashcode + hashFunction(e.vertex2);
		// hashcode = multiplier * hashcode + std::hash(e.weight);
		return hashcode;
	}
};
