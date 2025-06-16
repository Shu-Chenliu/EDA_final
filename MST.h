#ifndef MST_H
#define MST_H
#include <vector>
#include <set>
#include "classes.cpp"
using namespace std;  
class MST {
public:
    MST(const vector<FF*>& flip_flops, const vector<Cluster>& clusters);
    ~MST();
private:
    vector<FF*> flip_flops;
    vector<Cluster> clusters;
    vector<Edge> edges; // Store edges for MST
    vector<Edge> mst_edges; // Store edges in the MST
    unordered_map<string, int> ff_index_map; // Map FF names to indices
    void buildEdges();
    void kruskalMST();
    int findParent(int i, vector<int>& parent);
    void unionSets(int x, int y, vector<int>& parent, vector<int>& rank);
};
pair<int, pair<set<string>, set<string>>> cost(set<string> c) {
    int totalCost = 0;
    set<string> fanins, fanouts;
    for (const auto& ff : c) {
        FF* flipflop = map[ff];
        if (!flipflop) continue; // Skip if not found
        for (const auto& pin : flipflop->fanins) {
            fanins.insert(pin.name);
        }
        for (const auto& pin : flipflop->fanouts) {
            fanouts.insert(pin.name);
        }
    }
    totalCost = fanins.size() + fanouts.size();
    return {totalCost, {fanins, fanouts}};
}
    
