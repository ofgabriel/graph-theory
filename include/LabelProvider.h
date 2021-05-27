#pragma once

#include <string>
#include <map>

using namespace std;

class LabelProvider
{
public:
    LabelProvider(string filePath);
    string getLabel(int vertexId);
    int getFromLabel(string label);

private:
    map<int, string> m_LabelsMap;
};