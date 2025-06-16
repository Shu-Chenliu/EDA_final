#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Structure to represent an edge
struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Disjoint Set Union (Union-Find)
class DSU {
    vector<int> parent, rank;

public:
    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for(int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if(parent[x] != x)
            parent[x] = find(parent[x]); // Path compression
        return parent[x];
    }

    bool unite(int x, int y) {
        int xr = find(x);
        int yr = find(y);
        if(xr == yr) return false;

        // Union by rank
        if(rank[xr] < rank[yr]) {
            parent[xr] = yr;
        } else if(rank[xr] > rank[yr]) {
            parent[yr] = xr;
        } else {
            parent[yr] = xr;
            rank[xr]++;
        }
        return true;
    }
};

// Kruskal's algorithm
int kruskal(int n, vector<Edge>& edges, vector<Edge>& mst) {
    sort(edges.begin(), edges.end()); // Sort edges by weight
    DSU dsu(n);
    int total_weight = 0;

    for(const Edge& e : edges) {
        if(dsu.unite(e.u, e.v)) {
            mst.push_back(e);
            total_weight += e.weight;
        }
    }
    return total_weight;
}

// Example usage
int main() {
    int n = 5; // Number of nodes (0-based index)
    vector<Edge> edges = {
        {0, 1, 10},
        {0, 2, 6},
        {0, 3, 5},
        {1, 3, 15},
        {2, 3, 4}
    };

    vector<Edge> mst;
    int weight = kruskal(n, edges, mst);

    cout << "Total weight of MST: " << weight << endl;
    cout << "Edges in MST:\n";
    for(const Edge& e : mst) {
        cout << e.u << " - " << e.v << " : " << e.weight << endl;
    }

    return 0;
}
