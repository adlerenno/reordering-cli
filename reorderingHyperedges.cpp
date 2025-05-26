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

void reconstructGraph_reordering_hyperedges(vector<vector<int>> &hyperEdge, unordered_map<int, vector<int>> &hyperNode, vector<Vertex> &vertexSet, vector<int> &hyperedgeSet, vector<Edge> &edgeID, vector<int> &edgeSet, int w)
{
    set<PII, myCmp> remainSet;
    unordered_map<int, int> key;
    unordered_map<int, bool> visitHyperedge;
    int pos = -1, cardi = -1;
    for (int i = 0; i < int(hyperEdge.size()); ++i)
    {
        remainSet.insert(make_pair(i, 0));
        key[i] = 0;
        visitHyperedge[i] = false;
        if (int(hyperEdge[i].size()) > cardi)
        {
            pos = i;
            cardi = int(hyperEdge[i].size());
        }
    }
    Edge tmp;
    tmp.ID = pos;
    tmp.posStar = 0;
    tmp.posEnd = 0;
    edgeID.push_back(tmp);
    remainSet.erase(make_pair(pos, key[pos]));
    visitHyperedge[pos] = true;
    int i = 1;
    while (i < int(hyperEdge.size()))
    {
        int ve = edgeID[i - 1].ID;
        for (auto &node : hyperEdge[ve])
        {
            for (auto &edge : hyperNode[node])
            {
                if (!visitHyperedge[edge])
                {
                    if (remainSet.erase(make_pair(edge, key[edge])))
                    {
                        ++key[edge];
                        remainSet.insert(make_pair(edge, key[edge]));
                    }
                }
            }
        }
        if (i > w - 1)
        {
            int vs = edgeID[i - w].ID;
            for (auto &node : hyperEdge[vs])
            {
                for (auto &edge : hyperNode[node])
                {
                    if (!visitHyperedge[edge])
                    {
                        if (remainSet.erase(make_pair(edge, key[edge])))
                        {
                            --key[edge];
                            remainSet.insert(make_pair(edge, key[edge]));
                        }
                    }
                }
            }
        }
        PII p = *remainSet.begin();
        remainSet.erase(remainSet.begin());
        Edge tmp;
        tmp.ID = p.first;
        tmp.posStar = 0;
        tmp.posEnd = 0;
        edgeID.push_back(tmp);
        visitHyperedge[p.first] = true;
        ++i;
    }
    // process hyperedges
    for (int i = 0; i < int(edgeID.size()); i++)
    {
        unordered_map<int, int> tmp;
        if (i == int(edgeID.size()) - 1)
        {
            for (auto &node : hyperEdge[edgeID[i].ID])
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

        for (auto &node : hyperEdge[edgeID[i].ID])
            tmp[node]++;
        if (i > 0)
        {
            for (int j = edgeID[i].posStar; j < edgeID[i - 1].posEnd; j++)
                tmp.erase(tmp.find(edgeSet[j]));
        }
        for (auto &node : hyperEdge[edgeID[i + 1].ID])
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
    // process vertices
    vertexSet.resize(int(hyperNode.size()));;
    for (int i = 0; i < int(hyperNode.size()); i++)
    {
        vertexSet[i].ID = i;
        unordered_map<int, int> tmp;
        if (i == int(hyperNode.size()) - 1)
        {
            for (auto &edge : hyperNode[i])
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
        for (auto &edge : hyperNode.at(i))
            tmp[edge]++;
        if (i > 0)
        {
            for (int j = vertexSet[i].posStar; j < vertexSet[i - 1].posEnd; j++)
                tmp.erase(tmp.find(hyperedgeSet[j]));
        }
        for (auto &edge : hyperNode.at(i + 1))
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
    vertexSet[0].posStar = 0;
}

void output_reordering_hyperedges(const string &str, const bool compressed_out, vector<vector<int>> &hyperEdge, unordered_map<int, vector<int>> &hyperNode, vector<Vertex> &vertexSet, vector<int> &hyperedgeSet, vector<Edge> &edgeID, vector<int> &edgeSet, int w)
{
    unordered_map<int, int> ID;
    // convert ID
    for (int i = 0; i < int(edgeID.size()); i++)
    {
        ID[edgeID[i].ID] = i;
    }
    vector<vector<int>> pairs;
    for (int i = 0; i < int(hyperEdge.size()); i++)
    {
        pairs.push_back(hyperEdge[edgeID[i].ID]);
    }
    for (int i = 0; i < int(hyperEdge.size()); i++)
    {
        hyperEdge[i] = pairs[i];
    }
    for (auto &info : edgeID)
    {
        info.ID = ID[info.ID];
    }
    for (int i = 0; i < int(hyperedgeSet.size()); i++)
    {
        hyperedgeSet[i] = ID[hyperedgeSet[i]];
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

void solve_reordering_hyperedges(const string &input_file, const string &output_file, const bool compressed_out)
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
    reconstructGraph_reordering_hyperedges(hyperEdge, hyperNode, vertexSet, hyperedgeSet, edgeID, edgeSet, w);
    output_reordering_hyperedges(output_file, compressed_out, hyperEdge, hyperNode, vertexSet, hyperedgeSet, edgeID, edgeSet, w);
#pragma omp critical
    {
        cout << "file:" << input_file << "done!" << endl;
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
//    for (int i = 0; i < int(files.size()); i++)
//    {
//        solve(files[i]);
//    }
//    cout << "All done!" << endl;
//}
