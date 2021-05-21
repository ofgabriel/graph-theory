#pragma once

struct Edge
{
	int neighbor;
	float weight;

	Edge(int neighbor, float weight)
	{
		this->neighbor = neighbor;
		this->weight = weight;
	}

    bool operator < (const Edge& other) const
    {
        return (neighbor < other.neighbor);
    }

    bool operator > (const Edge& other) const
    {
        return (weight > other.weight);
    }
};
