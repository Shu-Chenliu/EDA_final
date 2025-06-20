#ifndef MST_H
#define MST_H
#include <vector>
#include <algorithm>
using namespace std;
// Structure to represent an edge
struct Edge {
    int start, end, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
    Edge(int s, int e, int w) : start(s), end(e), weight(w) {}
};
// Disjoint Set Union (DSU) or Union-Find structure
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

class MST{
public:
    MST(vector<Edge> &edges, int n) : edges(edges), n(n) {}
    ~MST() {}
    double MinimumSpanningTreeCost();
private:
    vector<Edge> edges;
    int n; // Number of nodes

    
};
#endif