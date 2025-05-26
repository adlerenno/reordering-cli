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
using namespace std;

void do_query_incidence_list(vector<vector<int>> &hyperEdge, vector<vector<int>> &hyperNode, int v)
{
    vector<bool> visited(int(hyperNode.size()), false);
    for (int i = 0; i < int(hyperNode[v].size()); i++)
    {
        int e = hyperNode[v][i];
        //cout << "Edge " << e << ": ";
        for (int j = 0; j < int(hyperEdge[e].size()); j++)
        {
            int u = hyperEdge[e][j];
            //   cout << u << ", ";
            visited[u] = true;
        }
        // cout << endl;
    }
    int sum = 0;
    for (int i = 0; i < int(hyperNode.size()); i++)
    {
        if (visited[i])
            sum++;
    }
    cout << "Found " << sum << " nodes connected." << endl;
}