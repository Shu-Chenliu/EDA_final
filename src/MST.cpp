#include <iostream>
#include <vector>
#include <algorithm>
#include "MST.h"
using namespace std;


// Kruskal's algorithm
double kruskal(int n, vector<Edge>& edges, vector<Edge>& mst) {
    sort(edges.begin(), edges.end()); // Sort edges by weight
    DSU dsu(n);
    double total_weight = 0;

    for(const Edge& e : edges) {
        if(dsu.unite(e.start, e.end)) {
            mst.push_back(e);
            total_weight += e.weight;
        }
    }
    return total_weight;
}

// Example usage
double MST::MinimumSpanningTreeCost() {
    
    vector<Edge> mst;
    double weight = kruskal(n, edges, mst);

    // cout << "Total weight of MST: " << weight << endl;
    // cout << "Edges in MST:\n";
    // for(const Edge& e : mst) {
    //     cout << e.start << " - " << e.end << " : " << e.weight << endl;
    // }

    return weight;
}
