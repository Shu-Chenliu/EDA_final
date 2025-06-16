#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <set>
#include "MBFFgeneration.h"
#include "kmean.h"


using namespace std;

kmean::kmean(int SIZE_LIMIT,int MAX_ITER,int DISP_LIMIT):SIZE_LIMIT(SIZE_LIMIT),MAX_ITER(MAX_ITER),DISP_LIMIT(DISP_LIMIT){}
kmean::~kmean(){}
double manhattanDist(const FF* p, const Cluster& c) {
    return abs(p->getX() - c.cx) + abs(p->getY() - c.cy);
}

double kmean::weightedCost(const FF* p, const Cluster& c) {
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
            sumX += ff->getX();
            sumY += ff->getY();
        }
        c.cx = sumX / c.flip_flops.size();
        c.cy = sumY / c.flip_flops.size();
    }
}

void kmean::assignPoints(vector<FF*>& flip_flops, vector<Cluster>& clusters, bool weighted = true) {
    for (auto& c : clusters)
        c.flip_flops.clear(); // clear assigned flip_flops for each cluster

    for (size_t i = 0; i < flip_flops.size(); ++i) {
        double minCost = numeric_limits<double>::max();
        int best = -1;
        for (size_t k = 0; k < clusters.size(); ++k) {
            double cost = weighted ? weightedCost(flip_flops[i], clusters[k]) : manhattanDist(flip_flops[i], clusters[k]); // decide cost function based on weighted or unweighted
            if (cost < minCost) {
                minCost = cost;
                best = k;
            }
        }
        flip_flops[i]->setCluster(best);
        clusters[best].flip_flops.push_back(flip_flops[i]);
    }
}

void initCentersRecursive(const vector<FF*>& flip_flops, vector<Cluster>& clusters, vector<int> indices, int K, bool splitX = true) {
    if (K == 1) {
        double avgX = 0, avgY = 0;
        for (int i : indices) {
            avgX += flip_flops[i]->getX();
            avgY += flip_flops[i]->getY();
        }
        avgX /= indices.size();
        avgY /= indices.size();
        clusters.push_back({avgX, avgY});
        return;
    }

    sort(indices.begin(), indices.end(), [&](int a, int b) {
        return splitX ? (flip_flops[a]->getX() < flip_flops[b]->getX()) : (flip_flops[a]->getY() < flip_flops[b]->getY());
    });

    int mid = indices.size() / 2;
    vector<int> left(indices.begin(), indices.begin() + mid);
    vector<int> right(indices.begin() + mid, indices.end());

    initCentersRecursive(flip_flops, clusters, left, K / 2, !splitX);
    initCentersRecursive(flip_flops, clusters, right, K - K / 2, !splitX);
}

void initializeCenters(const vector<FF*>& flip_flops, vector<Cluster>& clusters, int K) {
    vector<int> indices(flip_flops.size());
    for (size_t i = 0; i < flip_flops.size(); ++i)
        indices[i] = i;
    initCentersRecursive(flip_flops, clusters, indices, K);
}

void kmean::resolveOverflow(vector<FF*>& flip_flops, vector<Cluster>& clusters) {
    for (size_t i = 0; i < clusters.size(); ++i) {
        if ((int)clusters[i].flip_flops.size() <= SIZE_LIMIT) continue; // Skip clusters that are within size limit

        Cluster newCluster = clusters[i];
        newCluster.cx += 1.0;
        newCluster.cy += 1.0;
        newCluster.flip_flops.clear();

        // if cluster exceeds size limit, move excess flip_flops to new cluster        
        while ((int)clusters[i].flip_flops.size() > SIZE_LIMIT) {
            FF* ff = clusters[i].flip_flops.back();
            clusters[i].flip_flops.pop_back();
            ff->setCluster(clusters.size());
            newCluster.flip_flops.push_back(ff);
        }

        clusters.push_back(newCluster);
    }
}

// Resolve over displacement
void kmean::resolveOverDisplacement(vector<FF*>& flip_flops, vector<Cluster>& clusters) {
    for (size_t i = 0; i < flip_flops.size(); ++i) {
        int cid = flip_flops[i]->getCluster();
        // Check if the flop is too far from its cluster center

        if (manhattanDist(flip_flops[i], clusters[cid]) > DISP_LIMIT) {
            // New cluster centered at violating flop
            Cluster newC = {flip_flops[i]->getX(), flip_flops[i]->getY()};
            newC.flip_flops.push_back(flip_flops[i]);
            flip_flops[i]->setCluster(clusters.size());
            clusters.push_back(newC);
            
            // Remove flop from old cluster
            auto& oldCluster = clusters[cid];
            auto it = find(oldCluster.flip_flops.begin(), oldCluster.flip_flops.end(), flip_flops[i]);
            if (it != oldCluster.flip_flops.end()) {
                oldCluster.flip_flops.erase(it);
            }
            // Update cluster center
            oldCluster.cx = 0;
            oldCluster.cy = 0;
            for (FF* ff : oldCluster.flip_flops) {
                oldCluster.cx += ff->getX();
                oldCluster.cy += ff->getY();
            }
            if (!oldCluster.flip_flops.empty()) {
                oldCluster.cx /= oldCluster.flip_flops.size();
                oldCluster.cy /= oldCluster.flip_flops.size();
            } else {
                // If the cluster is empty, reset center to origin
                oldCluster.cx = 0;
                oldCluster.cy = 0;
            }
            // Update new cluster center
            newC.cx = flip_flops[i]->getX();
            newC.cy = flip_flops[i]->getY();
            for (FF* ff : newC.flip_flops) {
                newC.cx += ff->getX();
                newC.cy += ff->getY();
            }
            if (!newC.flip_flops.empty()) {
                newC.cx /= newC.flip_flops.size();
                newC.cy /= newC.flip_flops.size();
            } else {
                // If the new cluster is empty, reset center to origin
                newC.cx = 0;
                newC.cy = 0;
            }
            // Update the cluster in the vector
            clusters[cid] = oldCluster;
            clusters.push_back(newC); // Add the new cluster to the list
            cout << "Flop " << flip_flops[i]->getName() << " moved to new cluster " << clusters.size() - 1
                 << " due to over displacement from cluster " << cid << "\n";

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
        sort(c.flip_flops.begin(), c.flip_flops.end(), [&](FF* a, FF* b) {
            return manhattanDist(a, c) > manhattanDist(b, c);
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
                c.flip_flops[i]->setRelocateCoor(Coor(rx,ry));
                // c.flip_flops[i]->relocatedX = rx;
                // c.flip_flops[i]->relocatedY = ry;
                used.insert({rx, ry});
                ++i;
            }
        }
    }
}

vector<Cluster> kmean::kmeansWeighted(vector<FF*>& flip_flops) {
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
    double currentCost = numeric_limits<double>::max();
    double totalCost = 0;
    // vector<FF*> oldPoints = flip_flops; // Store original flip_flops for relocation
    while (update &&  iter < MAX_ITER)  // Continue until no updates or max iterations
    {
        /* code */
        iter++;
        // record each ff's original cluster
        vector<FF*> oldPoints = flip_flops; // Store original flip_flops for relocation
        
        // print current flip_flops' clusters
        
        // for (size_t i = 0; i < flip_flops.size(); ++i) {
        //     cout << "Flop " << i << ": original=(" << flip_flops[i]->position.x << "," << flip_flops[i]->position.y
        //          << "), cluster=" << flip_flops[i]->cluster
        //          << ", relocated=(" << flip_flops[i]->relocatedX << "," << flip_flops[i]->relocatedY << ")\n";
        // }
        

        assignPoints(flip_flops, clusters, true); // weighted after first iteration
        resolveOverflow(flip_flops, clusters);
        updateCenters(flip_flops, clusters);
        update = 0;
        for (size_t i = 0; i < flip_flops.size(); ++i) {
            if (flip_flops[i]->getCluster() != oldPoints[i]->getCluster()) {
                update = 1; // If any point's cluster changed, we need another iteration
                
                i = flip_flops.size();
                // break;
            } 
        }
        // if (update) {
        //     cout << "Iteration " << iter << ": updated clusters\n";
        // } else {
        //     cout << "No updates in iteration " << iter << "\n";
        // }
        
        resolveOverDisplacement(flip_flops, clusters);
        relocateFlops(flip_flops, clusters);
        updateCenters(flip_flops, clusters);

        // print cost
        
        totalCost = 0;
        for (size_t i = 0; i < flip_flops.size(); ++i) {
            totalCost += weightedCost(flip_flops[i], clusters[flip_flops[i]->getCluster()]);
        }
        if (totalCost < currentCost) {
            update = 1; // If cost improved, we need another iteration
            cout << "Iteration " << iter << ": cost improved from " << currentCost << " to " << totalCost << "\n";
        } else {
            cout << "Iteration " << iter << ": cost did not improve (" << currentCost << ")\n";
        }
        currentCost = totalCost; // Update current cost for next iteration
        // cout << "Total cost after iteration " << iter << ": " << totalCost << "\n";
            
    }
    

    

    // resolveOverDisplacement(flip_flops, clusters);
    // relocateFlops(flip_flops, clusters);

    cout << "\nClusters after relocation:\n";
    for (size_t i = 0; i < clusters.size(); ++i){
        cout << "Cluster " << i << ": " << clusters[i].flip_flops.size() << " flops\n";
        for (FF* ff : clusters[i].flip_flops) {
            cout << "  " << ff->getName() << " at (" << ff->getRelocateCoor().getX() << ", " << ff->getRelocateCoor().getY() << ")\n";
        }
    }

    
    int totalMove = 0;   
    int totalDistToCluster = 0;

    for (size_t i = 0; i < flip_flops.size(); ++i) {
        cout << "Flop " << i << ": original=(" << flip_flops[i]->getX() << "," << flip_flops[i]->getY()
             << "), cluster=" << flip_flops[i]->getCluster()
             << ", relocated=(" << flip_flops[i]->getRelocateCoor().getX() << "," << flip_flops[i]->getRelocateCoor().getY() << ")\n";
        totalMove += abs(flip_flops[i]->getRelocateCoor().getX() - flip_flops[i]->getX()) + abs(flip_flops[i]->getRelocateCoor().getY() - flip_flops[i]->getY());
        totalDistToCluster += manhattanDist(flip_flops[i], clusters[flip_flops[i]->getCluster()]);
    }
    cout << "Total distance relocated to cluster centers: " << totalMove << endl;
    cout << "Total distance to cluster centers: " << totalDistToCluster << endl;
    return clusters;
}


