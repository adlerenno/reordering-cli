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
#include <thread>
#include <mutex>
#ifdef USE_OPENMP
#include <omp.h>
#endif

#include "get_graph.h"
using namespace std;

typedef pair<int, int> PII;

struct myCmp
{
    bool operator()(const PII &a, const PII &b) const
    {
        if (a.first == b.first)
            return false;
        else
        {
            if (a.second != b.second)
                return a.second > b.second;
            else
                return a.first < b.first;
        }
    }
};

void reconstructGraph_reordering_vertices(vector<vector<int>> &hyperEdge, unordered_map<int, vector<int>> &hyperNode, vector<Vertex> &vertexSet, vector<int> &hyperedgeSet, vector<Edge> &edgeID, vector<int> &edgeSet, int w)
{
    // Construct a set that returns the minimum value each time and Pick a vertex with the largest degree
    set<PII, myCmp> remainSet;
    int pos = -1, deg = -1;
    unordered_map<int, int> key;
    unordered_map<int, bool> visitVertex;
    for (auto &it : hyperNode)
    {
        remainSet.insert(make_pair(it.first, 0));
        key[it.first] = 0;
        visitVertex[it.first] = false;
        // Pick a vertex with the largest degree
        if (int(it.second.size()) > deg)
        {
            pos = it.first;
            deg = int(it.second.size());
        }
    }

    // Priority solution vertex set
    Vertex tmp;
    tmp.ID = pos;
    tmp.posStar = 0;
    tmp.posEnd = 0;
    vertexSet.push_back(tmp);
    remainSet.erase(make_pair(pos, key[pos]));
    visitVertex[pos] = true;

    int i = 1;
    while (i < int(hyperNode.size()))
    {
        // cout << str << "\t num = " << hyperNode.size() << "\ti= " << i << endl;
        int ve = vertexSet[i - 1].ID;
        for (auto &edge : hyperNode[ve])
        {
            for (auto &node : hyperEdge[edge])
            {
                if (!visitVertex[node])
                {
                    if (remainSet.erase(make_pair(node, key[node])))
                    {
                        ++key[node];
                        remainSet.insert(make_pair(node, key[node]));
                    }
                    for (auto &edge1 : hyperNode[node])
                    {
                        if (edge1 != edge)
                        {
                            for (auto &node1 : hyperEdge[edge1])
                            {
                                if (node1 != node && !visitVertex[node1])
                                {
                                    if (remainSet.erase(make_pair(node1, key[node1])))
                                    {
                                        ++key[node1];
                                        remainSet.insert(make_pair(node1, key[node1]));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (i > w - 1)
        {
            int vs = vertexSet[i - w].ID;
            for (auto &edge : hyperNode[vs])
            {
                for (auto &node : hyperEdge[edge])
                {
                    if (!visitVertex[node])
                    {
                        if (remainSet.erase(make_pair(node, key[node])))
                        {
                            --key[node];
                            remainSet.insert(make_pair(node, key[node]));
                        }

                        for (auto &edge1 : hyperNode[node])
                        {
                            if (edge1 != edge)
                            {
                                for (auto &node1 : hyperEdge[edge1])
                                {
                                    if (node1 != node && !visitVertex[node1])
                                    {
                                        if (remainSet.erase(make_pair(node1, key[node1])))
                                        {
                                            --key[node1];
                                            remainSet.insert(make_pair(node1, key[node1]));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        PII p = *remainSet.begin();
        remainSet.erase(remainSet.begin());
        Vertex tmp;
        tmp.ID = p.first;
        tmp.posStar = 0;
        tmp.posEnd = 0;
        vertexSet.push_back(tmp);
        visitVertex[p.first] = true;
        ++i;
    }
    // process hyperedgeSet
    for (int i = 0; i < int(vertexSet.size()); i++)
    {
        unordered_map<int, int> tmp;
        if (i == int(vertexSet.size()) - 1)
        {
            for (auto &edge : hyperNode[vertexSet[i].ID])
                tmp[edge]++;
            if (i > 0)
            {
                for (int j = vertexSet[i].posStar; j < vertexSet[i - 1].posEnd; j++)
                    tmp.erase(tmp.find(hyperedgeSet[j]));
            }
            for (auto it = tmp.begin(); it != tmp.end(); it++)
            {
                hyperedgeSet.push_back(it->first);
            }
            vertexSet[i].posEnd = int(hyperedgeSet.size());
            break;
        }

        for (auto &edge : hyperNode[vertexSet[i].ID])
            tmp[edge]++;
        if (i > 0)
        {
            for (int j = vertexSet[i].posStar; j < vertexSet[i - 1].posEnd; j++)
                tmp.erase(tmp.find(hyperedgeSet[j]));
        }
        for (auto &edge : hyperNode[vertexSet[i + 1].ID])
        {
            if (tmp.find(edge) != tmp.end())
            {
                tmp[edge]++;
            }
        }
        for (auto it = tmp.begin(); it != tmp.end(); it++)
        {
            if (it->second == 1)
            {
                hyperedgeSet.push_back(it->first);
            }
        }
        vertexSet[i + 1].posStar = int(hyperedgeSet.size());
        for (auto it = tmp.begin(); it != tmp.end(); it++)
        {
            if (it->second == 2)
            {
                hyperedgeSet.push_back(it->first);
            }
        }
        vertexSet[i].posEnd = int(hyperedgeSet.size());
    }

    // The vertices in the set of edges that make up each one
    edgeID.resize(int(hyperEdge.size()));
    for (int i = 0; i < int(hyperEdge.size()); i++)
    {
        edgeID[i].ID = i;
        unordered_map<int, int> tmp;
        if (i == int(hyperEdge.size()) - 1)
        {
            for (auto &node : hyperEdge[i])
                tmp[node]++;
            if (i > 0)
            {
                for (int j = edgeID[i].posStar; j < edgeID[i - 1].posEnd; j++)
                    tmp.erase(tmp.find(edgeSet[j]));
            }
            for (auto it = tmp.begin(); it != tmp.end(); it++)
            {
                edgeSet.push_back(it->first);
            }
            edgeID[i].posEnd = int(edgeSet.size());
            break;
        }

        for (auto &node : hyperEdge[i])
            tmp[node]++;
        if (i > 0)
        {
            for (int j = edgeID[i].posStar; j < edgeID[i - 1].posEnd; j++)
                tmp.erase(tmp.find(edgeSet[j]));
        }
        for (auto &node : hyperEdge[i + 1])
        {
            if (tmp.find(node) != tmp.end())
            {
                tmp[node]++;
            }
        }
        for (auto it = tmp.begin(); it != tmp.end(); it++)
        {
            if (it->second == 1)
            {
                edgeSet.push_back(it->first);
            }
        }
        edgeID[i + 1].posStar = int(edgeSet.size());
        for (auto it = tmp.begin(); it != tmp.end(); it++)
        {
            if (it->second == 2)
            {
                edgeSet.push_back(it->first);
            }
        }
        edgeID[i].posEnd = int(edgeSet.size());
    }
    edgeID[0].posStar = 0;
}

void output_reordering_vertices(const string &str, const bool compressed_out, vector<vector<int>> &hyperEdge, unordered_map<int, vector<int>> &hyperNode, vector<Vertex> &vertexSet, vector<int> &hyperedgeSet, vector<Edge> &edgeID, vector<int> &edgeSet, int w)
{
    unordered_map<int, int> id2id;
    // convert ID
    for (int i = 0; i < int(vertexSet.size()); i++)
    {
        id2id[vertexSet[i].ID] = i;
    }

    for (int i = 0; i < int(hyperEdge.size()); i++)
    {
        for (int j = 0; j < int(hyperEdge[i].size()); j++)
        {
            hyperEdge[i][j] = id2id[hyperEdge[i][j]];
        }
    }
    for (auto &info : vertexSet)
    {
        info.ID = id2id[info.ID];
    }
    for (int i = 0; i < int(edgeSet.size()); i++)
    {
        edgeSet[i] = id2id[edgeSet[i]];
    }

    if (compressed_out) {
        ofstream fout(str + "-vertexSet");
        for (auto it: vertexSet) {
            fout << it.ID << ' ' << it.posStar << ' ' << it.posEnd << endl;
        }
        fout.close();

        fout.open(str + "-hyperedgeSet");
        for (auto it: hyperedgeSet) {
            fout << it << endl;
        }
        fout.close();

        fout.open(str + "-edgeID");
        for (auto it: edgeID) {
            fout << it.ID << ' ' << it.posStar << ' ' << it.posEnd << endl;
        }
        fout.close();
        fout.open(str + "-edgeSet");
        for (auto it: edgeSet) {
            fout << it << endl;
        }
        fout.close();
    }
    else {
        ofstream fout(str);
        for (int i = 0; i < int(hyperEdge.size()); i++) {
            for (int j = 0; j < int(hyperEdge[i].size()); j++) {
                fout << hyperEdge[i][j] << ' ';
            }
            fout << endl;
        }
        fout.close();
    }
}

void solve_reordering_vertices(const string &input_file, const string &output_file, const bool compressed_out)
{
#ifdef USE_OPENMP
    int thread_id = omp_get_thread_num();
    int num_threads = omp_get_num_threads();
#pragma omp critical
    {
        cout << "file:" << input_file << " thread_id: " << thread_id << " num_threads: " << num_threads << endl;
    }
#endif
#ifndef USE_OPENMP
    cout << "file:" << input_file <<  " num_threads: " << 1 << endl;
#endif
    vector<vector<int>> hyperEdge;
    unordered_map<int, vector<int>> hyperNode;
    getGraph(input_file, hyperEdge, hyperNode);
    vector<Vertex> vertexSet;
    vector<int> hyperedgeSet;
    vector<Edge> edgeID;
    vector<int> edgeSet;
    int w = 16;
    reconstructGraph_reordering_vertices(hyperEdge, hyperNode, vertexSet, hyperedgeSet, edgeID, edgeSet, w);
    output_reordering_vertices(input_file, compressed_out, hyperEdge, hyperNode, vertexSet, hyperedgeSet, edgeID, edgeSet, w);
    #pragma omp critical
    {
        cout << "file:" << "done!" << endl;
    }
}

//int main()
//{
//    string fileList = "TaAU MaAn WaTr ThAU ThMS CoMH CoGe CoDB";
//    std::vector<std::string> files;
//    std::istringstream iss(fileList);
//    std::string file;
//    while (std::getline(iss, file, ' '))
//    {
//        files.push_back(file);
//    }
//#pragma omp parallel for
//    for (int i=0;i<int(files.size());i++)
//    {
//        solve(files[i]);
//    }
//    cout << "All done!" << endl;
//}
