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
};
