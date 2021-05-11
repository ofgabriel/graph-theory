#pragma once

struct Edge
{
	int source;
	int destination;
	float weight;

	Edge(int source, int destination, float weight)
	{
		this->source = source;
		this->destination = destination;
		this->weight = weight;
	}
};