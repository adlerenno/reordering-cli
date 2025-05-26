//
// Created by Enno Adler on 26.05.25.
//
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "get_graph.h"

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

void getGraph_reordering_vertices_hyperedges(const string &filename, vector<vector<int>> &hyperEdge)
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
    }
    fin.close();
    if (hyperEdge.size() == 0){
        cerr << "Error opening file " << filename << ". No hyperedges loaded" << endl;
        exit(1);
    }
}
