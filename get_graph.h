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
void getGraph_reordering_vertices_hyperedges(const string &filename, vector<vector<int>> &hyperEdge, char separator);
void load_graph(const string &input_file, vector<Vertex> &vertexSet, vector<int> &hyperedgeSet, vector<Edge> &edgeID, vector<int> &edgeSet);
void load_incidence_graph(const string &str, vector<vector<int>> &hyperEdge, vector<vector<int>> &hyperNode);

#endif //REORDERING_GET_GRAPH_H
