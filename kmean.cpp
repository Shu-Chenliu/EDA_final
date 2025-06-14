#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <set>
#include "MBFFgeneration.h"
using namespace std;



int SIZE_LIMIT;
double DISP_LIMIT;
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
    for (size_t i = 0; i < flip_flops.size(); ++i)
        indices[i] = i;
    initCentersRecursive(flip_flops, clusters, indices, K);
}

void resolveOverflow(vector<FF*>& flip_flops, vector<Cluster>& clusters) {
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
            ff->cluster = clusters.size();
            newCluster.flip_flops.push_back(ff);
        }

        clusters.push_back(newCluster);
    }
}

// Resolve over displacement
void resolveOverDisplacement(vector<FF*>& flip_flops, vector<Cluster>& clusters) {
    for (size_t i = 0; i < flip_flops.size(); ++i) {
        int cid = flip_flops[i]->cluster;
        // Check if the flop is too far from its cluster center

        if (manhattanDist(flip_flops[i], clusters[cid]) > DISP_LIMIT) {
            // New cluster centered at violating flop
            Cluster newC = {flip_flops[i]->position.x, flip_flops[i]->position.y};
            newC.flip_flops.push_back(flip_flops[i]);
            flip_flops[i]->cluster = clusters.size();
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
                oldCluster.cx += ff->position.x;
                oldCluster.cy += ff->position.y;
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
            newC.cx = flip_flops[i]->position.x;
            newC.cy = flip_flops[i]->position.y;
            for (FF* ff : newC.flip_flops) {
                newC.cx += ff->position.x;
                newC.cy += ff->position.y;
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
            cout << "Flop " << flip_flops[i]->name << " moved to new cluster " << clusters.size() - 1
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
            if (flip_flops[i]->cluster != oldPoints[i]->cluster) {
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
            totalCost += weightedCost(flip_flops[i], clusters[flip_flops[i]->cluster]);
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
            cout << "  " << ff->name << " at (" << ff->relocatedX << ", " << ff->relocatedY << ")\n";
        }
    }

    
    int totalMove = 0;   
    int totalDistToCluster = 0;

    for (size_t i = 0; i < flip_flops.size(); ++i) {
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
        new FF(1, 10, 10, "ff0",
            { Pin{ Point(5, 5), 10.0, 2 } },            // fanins
            { Pin{ Point(15, 15), 8.0, 3 } }            // fanouts
        ),
        new FF(1, 12, 11, "ff1",
            { Pin{ Point(11, 10), 7.0, 1 } },
            { Pin{ Point(16, 16), 9.0, 2 } }
        ),
        new FF(1, 11, 9,  "ff2", 
            { Pin{ Point(6, 6), 5.0, 1 } },
            { Pin{ Point(14, 14), 6.0, 2 } }
        ),
        new FF(1, 9,  12, "ff3", 
            { Pin{ Point(4, 4), 3.0, 1 } },
            { Pin{ Point(13, 13), 4.0, 2 } }
        ),
        new FF(1, 15, 15, "ff4",
            { Pin{ Point(10, 10), 12.0, 2 } },
            { Pin{ Point(20, 20), 11.0, 3 } }
        ),
        new FF(1, 17, 16, "ff5",
            { Pin{ Point(14, 15), 10.0, 1 }, 
              Pin{ Point(18, 17), 9.0, 2 } },
            { Pin{ Point(22, 22), 8.0, 3 } }
        ),

        
        new FF(1, 18, 19, "ff6",
            { Pin{ Point(13, 14), 8.0, 1 }, 
              Pin{ Point(17, 18), 9.0, 2 },
              Pin{ Point(20, 21), 7.0, 3 } },
            { Pin{ Point(21, 22), 7.0, 2 },
              Pin{ Point(24, 25), 6.0, 3 } }
        ),          


        new FF(1, 100, 100, "ff7", 
            { Pin{ Point(95, 95), 90.0, 2 }, 
              Pin{ Point(95, 93), 90.0, 2 }, }, // fanins
            { Pin{ Point(105, 105), 85.0, 3 } } // fanouts
        ),
        new FF(1, 102, 101, "ff8", 
            { Pin{ Point(97, 96), 88.0, 1 } }, // fanins
            { Pin{ Point(106, 107), 84.0, 2 } } // fanouts
        ),      
        new FF(1, 101, 99, "ff9", 
            { Pin{ Point(96, 94), 87.0, 1 } }, // fanins
            { Pin{ Point(104, 103), 83.0, 2 } } // fanouts
        ),
        new FF(1, 99, 102, "ff10", 
            { Pin{ Point(95, 93), 86.0, 1 } }, // fanins
            { Pin{ Point(103, 108), 82.0, 2 } } // fanouts
        ),
        new FF(1, 250, 250, "ff11", 
            { Pin{ Point(245, 245), 240.0, 2 } }, // fanins
            { Pin{ Point(255, 255), 235.0, 3 }, 
              Pin{ Point(255, 253), 235.0, 2 },
              Pin{ Point(255, 252), 235.0, 2 } } // fanouts
        ),
        new FF(1, 251, 249, "ff12", 
            { Pin{ Point(246, 244), 238.0, 1 } }, // fanins
            { Pin{ Point(256, 254), 234.0, 2 } } // fanouts
        ),
        new FF(1, 60, 305, "ff13", 
            { Pin{ Point(55, 300), 50.0, 2 } }, // fanins
            { Pin{ Point(65, 310), 45.0, 3 } } // fanouts
        ),
        new FF(1, 62, 307, "ff14", 
            { Pin{ Point(57, 302), 48.0, 1 } }, // fanins
            { Pin{ Point(67, 312), 43.0, 2 } } // fanouts
        ),
        new FF(1, 61, 303, "ff15", 
            { Pin{ Point(56, 301), 49.0, 1 } }, // fanins
            { Pin{ Point(66, 308), 44.0, 2 } } // fanouts
        ),
        new FF(1, 59, 308, "ff16", 
            { Pin{ Point(54, 306), 51.0, 1 } }, // fanins
            { Pin{ Point(64, 313), 46.0, 2 } } // fanouts
        ),
        new FF(1, 80, 405, "ff17", 
            { Pin{ Point(75, 400), 70.0, 2 } }, // fanins
            { Pin{ Point(85, 410), 65.0, 3 } } // fanouts
        ),
        new FF(1, 82, 407, "ff18", 
            { Pin{ Point(77, 402), 68.0, 1 } }, // fanins
            { Pin{ Point(87, 412), 63.0, 2 } } // fanouts
        ),
        new FF(1, 81, 403, "ff19", 
            { Pin{ Point(76, 401), 69.0, 1 } }, // fanins
            { Pin{ Point(86, 408), 64.0, 2 } } // fanouts
        ),
        new FF(1, 79, 408, "ff20", 
            { Pin{ Point(74, 406), 71.0, 1 } }, // fanins
            { Pin{ Point(84, 413), 66.0, 2 } } // fanouts
        ),
        new FF(1, 300, 300, "ff21", 
            { Pin{ Point(295, 295), 290.0, 2 } }, // fanins
            { Pin{ Point(305, 305), 285.0, 3 } } // fanouts
        ),
        new FF(1, 302, 301, "ff22", 
            { Pin{ Point(297, 296), 288.0, 1 } }, // fanins
            { Pin{ Point(306, 307), 284.0, 2 }, 
                Pin{ Point(306, 306), 284.0, 2 },
                Pin{ Point(306, 305), 284.0, 2 } } // fanouts
        ),
        new FF(1, 301, 299, "ff23", 
            { Pin{ Point(296, 294), 287.0, 1 } }, // fanins
            { Pin{ Point(305, 304), 283.0, 2 } } // fanouts
        ),
        new FF(1, 299, 302, "ff24", 
            { Pin{ Point(295, 293), 286.0, 1 } }, // fanins
            { Pin{ Point(304, 308), 282.0, 2 } } // fanouts
        ),
        new FF(1, 400, 400, "ff25", 
            { Pin{ Point(395, 395), 390.0, 2 } }, // fanins
            { Pin{ Point(405, 405), 385.0, 3 } } // fanouts
        ),
        new FF(1, 402, 401, "ff26", 
            { Pin{ Point(397, 396), 388.0, 1 } }, // fanins
            { Pin{ Point(406, 407), 384.0, 2 }, 
              Pin{ Point(406, 406), 384.0, 2 },
              Pin{ Point(406, 405), 384.0, 2 } } // fanouts
                
        ),
        new FF(1, 401, 399, "ff27", 
            { Pin{ Point(396, 394), 387.0, 1 }, 
              Pin{ Point(398, 398), 386.0, 1 }, 
              Pin{ Point(400, 400), 385.0, 1 } }, // fanins
            { Pin{ Point(405, 404), 383.0, 2 } } // fanouts
        ),
        new FF(1, 399, 402, "ff28", 
            { Pin{ Point(395, 393), 386.0, 1 } }, // fanins
            { Pin{ Point(404, 408), 382.0, 2 } } // fanouts
        )
        
    };
    // for (size_t i = 0; i < flip_flops.size(); ++i) {
    //     cout << "Flop " << i << ": original=(" << flip_flops[i]->position.x << "," << flip_flops[i]->position.y
    //          << "), cluster=" << flip_flops[i]->cluster
    //          << ", relocated=(" << flip_flops[i]->relocatedX << "," << flip_flops[i]->relocatedY << ")\n";
    // }
    int left = 0, right = 0, top = 0, bottom = 0;
    for (auto ff : flip_flops) {
        if (ff->position.x < left) left = ff->position.x;
        if (ff->position.x > right) right = ff->position.x;
        if (ff->position.y < top) top = ff->position.y;
        if (ff->position.y > bottom) bottom = ff->position.y;
    }
    SIZE_LIMIT = flip_flops.size() / 3 ; // Example size limit for clusters
    MAX_ITER = flip_flops.size() * 2; // Example maximum iterations
    DISP_LIMIT = (right - left + bottom - top) / 3;
    
    vector<Cluster> clusters=kmeansWeighted(flip_flops);

    for(size_t i=0;i<1;i++){
        vector<FF*> flipflop=clusters[i].flip_flops;
        int maxDrivingStrength = 4;
        double beta = 0.95;
        MBFFgeneration generator(flipflop, maxDrivingStrength, beta);
        // vector<set<string>> mbff_result = generator.generateMBFF();
        vector<MBFF> placed_mbffs=generator.locationAssignment(Rect(0,441,0,448));
        generator.MBFFsizing(placed_mbffs);
    }
    for (auto ff : flip_flops) {
        delete ff;
    }
    flip_flops.clear();
    return 0;
}
