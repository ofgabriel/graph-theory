#include "LabelProvider.h"
#include <fstream>


LabelProvider::LabelProvider(string filePath)
{
    ifstream file;
    file.open(filePath);

	string line = "";
    while (std::getline(file, line))
	{
        int commaId = line.find(',');
		int vertex = stoi(line.substr(0, commaId));
		string label = line.substr(commaId + 1);
		m_LabelsMap.insert({vertex,  label});
	}
}

string LabelProvider::getLabel(int vertexId)
{
    auto it = m_LabelsMap.find(vertexId);
    if (it != m_LabelsMap.end())
    {
        return  it->second;
    }
    return nullptr;
}

int LabelProvider::getFromLabel(string label)
{
    for (auto vertex : m_LabelsMap)
    {
        if (vertex.second == label)
		{
			return vertex.first;
		}
    }
	return -1;
}

