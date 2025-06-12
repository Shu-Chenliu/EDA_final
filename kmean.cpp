// g++ kmean.cpp -o kmean
// .kmean

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <set>
#include <MBFFgeneration.h>
#include <classes.cpp>
using namespace std;



int SIZE_LIMIT;
const double DISP_LIMIT = 160.0;
int MAX_ITER;

double manhattanDist(const FF* p, const Cluster& c) {
    return abs(p->position.x - c.cx) + abs(p->position.y - c.cy);
}

double weightedCost(const FF* p, const Cluster& c) {
    int size = c.flip_flops.size(); // Size of the cluster
    double baseCost = manhattanDist(p, c); // distance to cluster center
    double weight = max(double(size) / SIZE_LIMIT, 1.0);
    return baseCost * weight;
}

void updateCenters(vector<FF*>& flip_flops, vector<Cluster>& clusters) {
    for (auto& c : clusters) {
        if (c.flip_flops.empty()) continue;
        double sumX = 0, sumY = 0;
        for (FF* ff : c.flip_flops) {
            sumX += ff->position.x;
            sumY += ff->position.y;
        }
        c.cx = sumX / c.flip_flops.size();
        c.cy = sumY / c.flip_flops.size();
    }
}

void assignPoints(vector<FF*>& flip_flops, vector<Cluster>& clusters, bool weighted = true) {
    for (auto& c : clusters)
        c.flip_flops.clear(); // clear assigned flip_flops for each cluster

    for (int i = 0; i < flip_flops.size(); ++i) {
        double minCost = numeric_limits<double>::max();
        int best = -1;
        for (int k = 0; k < clusters.size(); ++k) {
            double cost = weighted ? weightedCost(flip_flops[i], clusters[k]) : manhattanDist(flip_flops[i], clusters[k]); // decide cost function based on weighted or unweighted
            if (cost < minCost) {
                minCost = cost;
                best = k;
            }
        }
        flip_flops[i]->cluster = best;
        clusters[best].flip_flops.push_back(flip_flops[i]);
    }
}

void initCentersRecursive(const vector<FF*>& flip_flops, vector<Cluster>& clusters, vector<int> indices, int K, bool splitX = true) {
    if (K == 1) {
        double avgX = 0, avgY = 0;
        for (int i : indices) {
            avgX += flip_flops[i]->position.x;
            avgY += flip_flops[i]->position.y;
        }
        avgX /= indices.size();
        avgY /= indices.size();
        clusters.push_back({avgX, avgY});
        return;
    }

    sort(indices.begin(), indices.end(), [&](int a, int b) {
        return splitX ? (flip_flops[a]->position.x < flip_flops[b]->position.x) : (flip_flops[a]->position.y < flip_flops[b]->position.y);
    });

    int mid = indices.size() / 2;
    vector<int> left(indices.begin(), indices.begin() + mid);
    vector<int> right(indices.begin() + mid, indices.end());

    initCentersRecursive(flip_flops, clusters, left, K / 2, !splitX);
    initCentersRecursive(flip_flops, clusters, right, K - K / 2, !splitX);
}

void initializeCenters(const vector<FF*>& flip_flops, vector<Cluster>& clusters, int K) {
    vector<int> indices(flip_flops.size());
    for (int i = 0; i < flip_flops.size(); ++i)
        indices[i] = i;
    initCentersRecursive(flip_flops, clusters, indices, K);
}

void resolveOverflow(vector<FF*>& flip_flops, vector<Cluster>& clusters) {
    for (int i = 0; i < clusters.size(); ++i) {
        if (clusters[i].flip_flops.size() <= SIZE_LIMIT) continue;

        Cluster newCluster = clusters[i];
        newCluster.cx += 1.0;
        newCluster.cy += 1.0;
        newCluster.flip_flops.clear();

        // if cluster exceeds size limit, move excess flip_flops to new cluster        
        while (clusters[i].flip_flops.size() > SIZE_LIMIT) {
            FF* ff = clusters[i].flip_flops.back();
            clusters[i].flip_flops.pop_back();
            ff->cluster = clusters.size();
            newCluster.flip_flops.push_back(ff);
        }

        clusters.push_back(newCluster);
    }
}

// Resolve over displacement
void resolveOverDisplacement(vector<FF*>& flip_flops, vector<Cluster>& clusters) {
    for (int i = 0; i < flip_flops.size(); ++i) {
        int cid = flip_flops[i]->cluster;
        // Check if the flop is too far from its cluster center

        if (manhattanDist(flip_flops[i], clusters[cid]) > DISP_LIMIT) {
            // New cluster centered at violating flop
            Cluster newC = {flip_flops[i]->position.x, flip_flops[i]->position.y};
            newC.flip_flops.push_back(flip_flops[i]);
            flip_flops[i]->cluster = clusters.size();
            clusters.push_back(newC);
        }
    }
}

// Relocate flops into grid structure around cluster center
void relocateFlops(vector<FF*>& flip_flops, vector<Cluster>& clusters) {
    for (auto& c : clusters) {
        if (c.flip_flops.empty()) continue; // Skip empty clusters

        int n = c.flip_flops.size();
        int width = ceil(sqrt(n));
        int height = (n + width - 1) / width;

        int px = round(c.cx);
        int py = round(c.cy);

        // Sort flops by criticality (distance to center, descending)
        sort(c.flip_flops.begin(), c.flip_flops.end(), [&](int a, int b) {
            return manhattanDist(flip_flops[a], c) > manhattanDist(flip_flops[b], c);
        });

        set<pair<int, int>> used;
        int dx = -width / 2;
        int dy = -height / 2;

        int i = 0;
        for (int row = 0; row < height && i < n; ++row) {
            for (int col = 0; col < width && i < n; ++col) {
                int rx = px + dx + col;
                int ry = py + dy + row;

                while (used.count({rx, ry})) rx++;  // ensure unique placement

                c.flip_flops[i]->relocatedX = rx;
                c.flip_flops[i]->relocatedY = ry;
                used.insert({rx, ry});
                ++i;
            }
        }
    }
}

vector<Cluster> kmeansWeighted(vector<FF*>& flip_flops) {
    int initialK = max(1, int(flip_flops.size() / SIZE_LIMIT)); // Initial number of clusters 
    vector<Cluster> clusters;
    initializeCenters(flip_flops, clusters, initialK);

    // for (int iter = 0; iter < MAX_ITER; ++iter) {
    //     assignPoints(flip_flops, clusters, iter > 0); // unweighted in first iteration
    //     resolveOverflow(flip_flops, clusters);
    //     updateCenters(flip_flops, clusters);
    // }
    int update = 1;
    int iter = 0;
    vector<FF*> oldPoints = flip_flops; // Store original flip_flops for relocation
    while (update && iter < MAX_ITER)  // Continue until no updates or max iterations
    {
        /* code */
        assignPoints(flip_flops, clusters, true); // weighted after first iteration
        resolveOverflow(flip_flops, clusters);
        updateCenters(flip_flops, clusters);
        update = 0;
        for (int i = 0; i < flip_flops.size(); ++i) {
            if (flip_flops[i]->cluster != oldPoints[i]->cluster) {
                update = 1; // If any point's cluster changed, we need another iteration
                iter++;
                i = flip_flops.size();
                // break;
            } 
        }
        if (update) {
            cout << "Iteration " << iter << ": updated clusters\n";
        } else {
            cout << "No updates in iteration " << iter << "\n";
        }
        
        resolveOverDisplacement(flip_flops, clusters);
        relocateFlops(flip_flops, clusters);
            
    }
    

    // resolveOverDisplacement(flip_flops, clusters);
    // relocateFlops(flip_flops, clusters);

    cout << "\nClusters after relocation:\n";
    for (int i = 0; i < clusters.size(); ++i)
        cout << "Cluster " << i << ": " << clusters[i].flip_flops.size() << " flops\n";
    
    int totalMove = 0;   
    int totalDistToCluster = 0;

    for (int i = 0; i < flip_flops.size(); ++i) {
        cout << "Flop " << i << ": original=(" << flip_flops[i]->position.x << "," << flip_flops[i]->position.y
             << "), cluster=" << flip_flops[i]->cluster
             << ", relocated=(" << flip_flops[i]->relocatedX << "," << flip_flops[i]->relocatedY << ")\n";
        totalMove += abs(flip_flops[i]->relocatedX - flip_flops[i]->position.x) + abs(flip_flops[i]->relocatedY - flip_flops[i]->position.y);
        totalDistToCluster += manhattanDist(flip_flops[i], clusters[flip_flops[i]->cluster]);
    }
    cout << "Total distance relocated to cluster centers: " << totalMove << endl;
    cout << "Total distance to cluster centers: " << totalDistToCluster << endl;
    return clusters;
}

int main() {
    vector<FF*> flip_flops = {
        new FF{10, 10}, new FF{12, 11}, new FF{11, 9}, new FF{9, 12},
        new FF{20, 20}, new FF{22, 21}, new FF{23, 19},
        new FF{100, 100}, new FF{101, 99}, new FF{99, 102}, new FF{102, 101},
        new FF{250, 250}, new FF{251, 249},
        new FF{60, 305}, new FF{62, 307}, new FF{61, 303}, new FF{59, 308},
        new FF{80, 405}, new FF{82, 407}, new FF{81, 403}, new FF{79, 408},
        new FF{300, 300}, new FF{302, 301}, new FF{301, 299}, new FF{299, 302},
        new FF{400, 400}, new FF{402, 401}, new FF{401, 399}, new FF{399, 402}
    };
    SIZE_LIMIT = flip_flops.size() / 3 ; // Example size limit for clusters
    MAX_ITER = flip_flops.size() * 2; // Example maximum iterations

    vector<Cluster> clusters=kmeansWeighted(flip_flops);
    for(int i=0;i<clusters.size();i++){
        vector<FF*> flipflop=clusters[i].flip_flops;
        int maxDrivingStrength = 4;
        double beta = 0.95;
        MBFFgeneration generator(flipflop, maxDrivingStrength, beta);
        vector<set<string>> mbff_result = generator.generateMBFF();
        vector<MBFF> placed_mbffs=generator.locationAssignment(Rect(-30,441,-31,448));
        generator.MBFFsizing(placed_mbffs);
    }
    for (auto ff : flip_flops) {
        delete ff;
    }
    flip_flops.clear();
    return 0;
}
