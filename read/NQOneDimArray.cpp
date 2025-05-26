#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <map>
#include <climits>
#include <algorithm>
#include <random>
#include <stack>
#include <thread>
#include <mutex>
#include "get_graph.h"
using namespace std;


void do_query_reordering(vector<Vertex> &vertexSet, vector<int> &hyperedgeSet, vector<Edge> &edgeID, vector<int> &edgeSet, int v)
{
    vector<bool> visited(int(vertexSet.size()), false);
    for (int i = vertexSet[v].posStar; i < vertexSet[v].posEnd; i++)
    {
        int e = hyperedgeSet[i];
        for (int j = edgeID[e].posStar; j < edgeID[e].posEnd; j++)
        {
            int u = edgeSet[j];
        }
    }
}
