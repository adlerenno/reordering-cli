//
// Created by Enno Adler on 26.05.25.
//

#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <string>
#include "query.hpp"
#include "get_graph.h"
#include "NQOneDimArray.hpp"
#include "NQAdjList.hpp"

using namespace std;

void perform_query(const string& input_file, const string& query_file, int type)
{
    if (type == 0)
    {
        vector<Vertex> vertexSet;
        vector<int> hyperedgeSet;
        vector<Edge> edgeID;
        vector<int> edgeSet;
        load_graph(input_file, vertexSet, hyperedgeSet, edgeID, edgeSet);

        cout << vertexSet.size()<<" "<<hyperedgeSet.size()<<" "<<edgeID.size()<<" "<<edgeSet.size()<<endl;
        int cnt = vertexSet.size() * 2 + hyperedgeSet.size() + edgeID.size() * 2 + edgeSet.size();
        cout << cnt << endl;

        std::ifstream query(query_file);
        std::string line;
        while (std::getline(query, line)) {
            std::stringstream ss;
            ss.str(line);
            uint64_t num;
            while (ss >> num) {
                do_query_reordering(vertexSet, hyperedgeSet, edgeID, edgeSet, num);
            }
        }
    }
    else
    {
        vector<vector<int>> hyperEdge;
        vector<vector<int>> hyperNode;
        load_incidence_graph(input_file, hyperEdge, hyperNode);
        std::ifstream query(query_file);
        std::string line;

        while (std::getline(query, line)) {
            std::stringstream ss;
            ss.str(line);
            uint64_t num;
            while (ss >> num) {
                do_query_incidence_list(hyperEdge, hyperNode, num);
            }
        }
    }

}
