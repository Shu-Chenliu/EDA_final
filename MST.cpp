#include <iostream>
#include <vector>
#include <algorithm>
#include "MST.h"
using namespace std;

// Structure to represent an edge


// Disjoint Set Union (Union-Find)


// Kruskal's algorithm
int kruskal(int n, vector<Edge>& edges, vector<Edge>& mst) {
    sort(edges.begin(), edges.end()); // Sort edges by weight
    DSU dsu(n);
    int total_weight = 0;

    for(const Edge& e : edges) {
        if(dsu.unite(e.start, e.end)) {
            mst.push_back(e);
            total_weight += e.weight;
        }
    }
    return total_weight;
}

// Example usage
int MST::MinimumSpanningTreeCost() {
    // int n = 5; // Number of nodes (0-based index)
    // vector<Edge> edges = {
    //     {0, 1, 10},
    //     {0, 2, 6},
    //     {0, 3, 5},
    //     {1, 3, 15},
    //     {2, 3, 4}
    // };


    vector<Edge> mst;
    int weight = kruskal(n, edges, mst);

    // cout << "Total weight of MST: " << weight << endl;
    // cout << "Edges in MST:\n";
    // for(const Edge& e : mst) {
    //     cout << e.start << " - " << e.end << " : " << e.weight << endl;
    // }

    return weight;
}
