//
// Created by Enno Adler on 26.05.25.
//
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

void getGraph(const string &filename, vector<vector<int>> &hyperEdge, unordered_map<int, vector<int>> &hyperNode)
{
    ifstream fin(filename, ios::in);
    if (!fin)
    {
        cerr << "Error opening file " << filename << endl;
        exit(1);
    }
    int count = -1;
    while (true)
    {
        string str;
        getline(fin, str);
        if (str == "")
            break;
        istringstream ss(str);
        vector<int> e;
        string token;
        while (getline(ss, token, ','))
        {
            int tmp = stoi(token);
            if (find(e.begin(), e.end(), tmp) == e.end())
                e.push_back(tmp);
        }
        if (e.size() == 1)
            continue;
        count++;
        hyperEdge.push_back(e);
        for (auto &node : e)
            hyperNode[node].push_back(count);
    }
    fin.close();
    if (hyperEdge.size() == 0){
        cerr << "Error opening file " << filename << ". No hyperedges loaded" << endl;
        exit(1);
    }
}

void getGraph_reordering_vertices_hyperedges(const string &filename, vector<vector<int>> &hyperEdge, char separator)
{

    ifstream fin(filename, ios::in);
    if (!fin)
    {
        cerr << "Error opening file " << filename << endl;
        exit(1);
    }
    int count = -1;
    while (true)
    {
        string str;
        getline(fin, str);
        if (str == "")
            break;
        istringstream ss(str);
        vector<int> e;
        string token;
        while (getline(ss, token, separator))
        {
            int tmp = stoi(token);
            if (find(e.begin(), e.end(), tmp) == e.end())
                e.push_back(tmp);
        }
        if (e.size() == 1)
            continue;
        count++;
        hyperEdge.push_back(e);
    }
    fin.close();
    if (hyperEdge.size() == 0){
        cerr << "Error opening file " << filename << ". No hyperedges loaded" << endl;
        exit(1);
    }
}

void load_graph(const string &input_file, vector<Vertex> &vertexSet, vector<int> &hyperedgeSet, vector<Edge> &edgeID, vector<int> &edgeSet)
{
    ifstream fin(input_file + "-vertexSet");
    while (true)
    {
        string str;
        getline(fin, str);
        if (str == "")
            break;
        istringstream ss(str);
        int a, b, c;
        ss >> a >> b >> c;
        Vertex tmp;
        tmp.ID = a;
        tmp.posStar = b;
        tmp.posEnd = c;
        vertexSet.push_back(tmp);
    }
    fin.close();

    fin.open(input_file + "-hyperedgeSet");
    int a;
    while (fin >> a)
    {
        hyperedgeSet.push_back(a);
    }
    fin.close();

    fin.open(input_file + "-edgeID");
    while (true)
    {
        string str;
        getline(fin, str);
        if (str == "")
            break;
        istringstream ss(str);
        int a, b, c;
        ss >> a >> b >> c;
        Edge tmp;
        tmp.ID = a;
        tmp.posStar = b;
        tmp.posEnd = c;
        edgeID.push_back(tmp);
    }
    fin.close();

    fin.open(input_file + "-edgeSet");
    while (fin >> a)
    {
        edgeSet.push_back(a);
    }
    fin.close();
}

void load_incidence_graph(const string &input_file, vector<vector<int>> &hyperEdge, vector<vector<int>> &hyperNode)
{
    ifstream fin(input_file, ios::in);
    int count = -1;
    unordered_map<int, vector<int>> tmpnode;
    int maxID = INT_MIN;
    while (true)
    {
        string str;
        getline(fin, str);
        if (str == "")
            break;
        istringstream ss(str);
        vector<int> e;
        string token;
        while (getline(ss, token, ','))
        {
            int tmp = stoi(token);
            if (find(e.begin(), e.end(), tmp) == e.end())
                e.push_back(tmp);
        }
        count++;
        hyperEdge.push_back(e);
        for (auto &node : e)
        {
            tmpnode[node].push_back(count);
            maxID = max(maxID, node);
        }
    }
    hyperNode.resize(maxID + 1);
    for (auto &info : tmpnode)
    {
        hyperNode[info.first] = info.second;
    }
}
