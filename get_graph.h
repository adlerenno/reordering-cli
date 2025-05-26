//
// Created by Enno Adler on 26.05.25.
//

#ifndef REORDERING_GET_GRAPH_H
#define REORDERING_GET_GRAPH_H

#include <vector>
#include <unordered_map>
using namespace std;
struct Vertex
{
    int ID;
    int posStar;
    int posEnd;
};

struct Edge
{
    int ID;
    int posStar;
    int posEnd;
};

void getGraph(const string &filename, vector<vector<int>> &hyperEdge, unordered_map<int, vector<int>> &hyperNode);

#endif //REORDERING_GET_GRAPH_H
