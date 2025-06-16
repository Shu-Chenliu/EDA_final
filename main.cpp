#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <set>
#include <random>  // For random device and engine
#include "MBFFgeneration.h"
#include "kmean.h"
#include "MST.h"


using namespace std;
int main() {
  random_device rd;
  mt19937 gen(rd());
  vector<FF*> flip_flops;

  // Step 1: 建立 FF 物件（先不加 fanins/fanouts）
  flip_flops.push_back(new FF(1, 1, 10, 10, "ff0"));
  flip_flops.push_back(new FF(1, 1, 12, 11, "ff1"));
  flip_flops.push_back(new FF(1, 1, 11, 9,  "ff2"));
  flip_flops.push_back(new FF(1, 1, 9,  12, "ff3"));
  flip_flops.push_back(new FF(1, 1, 15, 15, "ff4"));
  flip_flops.push_back(new FF(1, 1, 17, 16, "ff5"));
  flip_flops.push_back(new FF(1, 1, 18, 19, "ff6"));

  // Step 2: 為每個 FF 補上 fanins 與 fanouts 的 Pin，並填入 ff 指標
  flip_flops[0]->fanins = {
      Pin{ Point(5, 5), flip_flops[0], 10.0, 2 }
  };
  flip_flops[0]->fanouts = {
      Pin{ Point(15, 15), flip_flops[0], 8.0, 3 }
  };

  flip_flops[1]->fanins = {
      Pin{ Point(11, 10), flip_flops[1], 7.0, 1 }
  };
  flip_flops[1]->fanouts = {
      Pin{ Point(16, 16), flip_flops[1], 9.0, 2 }
  };

  flip_flops[2]->fanins = {
      Pin{ Point(6, 6), flip_flops[2], 5.0, 1 }
  };
  flip_flops[2]->fanouts = {
      Pin{ Point(14, 14), flip_flops[2], 6.0, 2 }
  };

  flip_flops[3]->fanins = {
      Pin{ Point(4, 4), flip_flops[3], 3.0, 1 }
  };
  flip_flops[3]->fanouts = {
      Pin{ Point(13, 13), flip_flops[3], 4.0, 2 }
  };

  flip_flops[4]->fanins = {
      Pin{ Point(10, 10), flip_flops[4], 12.0, 2 }
  };
  flip_flops[4]->fanouts = {
      Pin{ Point(20, 20), flip_flops[4], 11.0, 3 }
  };

  flip_flops[5]->fanins = {
      Pin{ Point(14, 15), flip_flops[5], 10.0, 1 },
      Pin{ Point(18, 17), flip_flops[5], 9.0, 2 }
  };
  flip_flops[5]->fanouts = {
      Pin{ Point(22, 22), flip_flops[5], 8.0, 3 }
  };

  flip_flops[6]->fanins = {
      Pin{ Point(13, 14), flip_flops[6], 8.0, 1 },
      Pin{ Point(17, 18), flip_flops[6], 9.0, 2 },
      Pin{ Point(20, 21), flip_flops[6], 7.0, 3 }
  };
  flip_flops[6]->fanouts = {
      Pin{ Point(21, 22), flip_flops[6], 7.0, 2 },
      Pin{ Point(24, 25), flip_flops[6], 6.0, 3 }
  };

  for(size_t i=0;i<flip_flops.size();i++){
    
    uniform_int_distribution<> num_next_dis(0, 3);
    int x = num_next_dis(gen);
    vector<int> indices;
    cout<<i<<":";
    for (size_t j = 0; j < flip_flops.size(); ++j) {
      if (j != i)
        indices.push_back(j);
    }

    // 打亂 index
    shuffle(indices.begin(), indices.end(), gen);

    // 取前 x 個當作 next
    for (int j = 0; j < x && j < (int)indices.size(); ++j) {
      flip_flops[i]->next.push_back(indices[j]);
      cout<<indices[j]<<" ";
    }
    cout<<endl;
  }
  // for (size_t i = 0; i < flip_flops.size(); ++i) {
  //     cout << "Flop " << i << ": original=(" << flip_flops[i]->position.x << "," << flip_flops[i]->position.y
  //          << "), cluster=" << flip_flops[i]->cluster
  //          << ", relocated=(" << flip_flops[i]->relocatedX << "," << flip_flops[i]->relocatedY << ")\n";
  // }
  // initial wire cost for TNS
  int total_wire_length = 0;
  vector<Edge> edges;
  // turn wire to edge
  for (size_t i = 0; i < flip_flops.size(); ++i) {
    for (size_t j = 0; j < flip_flops[i]->next.size(); ++j) {
      edges.push_back(Edge(
        i,
        flip_flops[i]->next[j],
        (int)abs(flip_flops[i]->position.x - flip_flops[flip_flops[i]->next[j]]->position.x) +
        abs(flip_flops[i]->position.y - flip_flops[flip_flops[i]->next[j]]->position.y)
      )); // Manhattan distance
      
    }
  }
  // do MST
  MST mst_before(edges, (int)flip_flops.size());
  total_wire_length = mst_before.MinimumSpanningTreeCost();
  cout << "Initial MST wire length: " << total_wire_length << endl;


  int left = 0, right = 0, top = 0, bottom = 0;
  for (auto ff : flip_flops) {
    if (ff->position.x < left) left = ff->position.x;
    if (ff->position.x > right) right = ff->position.x;
    if (ff->position.y < top) top = ff->position.y;
    if (ff->position.y > bottom) bottom = ff->position.y;
  }
  size_t size=flip_flops.size();
  for(size_t j=0;j<size*size;j++){
    uniform_int_distribution<> num_size((int)flip_flops.size() / 3, (int)flip_flops.size()*2 / 3);
    int SIZE_LIMIT = num_size(gen) ; // Example size limit for clusters
    int MAX_ITER = flip_flops.size() * 2; // Example maximum iterations
    uniform_int_distribution<> num_disp((int)(right - left + bottom - top) / 3, (int)(right - left + bottom - top) *2/ 3);
    int DISP_LIMIT = num_disp(gen);
    kmean kmean(SIZE_LIMIT,MAX_ITER,DISP_LIMIT);
    // update flip flops position
    

    vector<Cluster> clusters=kmean.kmeansWeighted(flip_flops);
    edges.clear();
    for (size_t i = 0; i < flip_flops.size(); ++i) {
      for (size_t j = 0; j < flip_flops[i]->next.size(); ++j) {
        edges.push_back(Edge(
          i,
          flip_flops[i]->next[j],
          (int)abs(flip_flops[i]->relocatedX - flip_flops[flip_flops[i]->next[j]]->relocatedX) +
          abs(flip_flops[i]->relocatedY - flip_flops[flip_flops[i]->next[j]]->relocatedY)
        )); // Manhattan distance
      }
    }
    // do MST
    MST mst_after(edges, (int)flip_flops.size());
    total_wire_length = mst_after.MinimumSpanningTreeCost();
    cout << "MST wire length after k-means: " << total_wire_length << endl;
    srand(time(0));
    for(size_t i=0;i<1;i++){
      vector<FF*> flipflop=clusters[i].flip_flops;
      int maxDrivingStrength = 4;
      double beta = 0.95;
      MBFFgeneration generator(flipflop, maxDrivingStrength, beta);
      // vector<set<string>> mbff_result = generator.generateMBFF();
      vector<MBFF> placed_mbffs=generator.locationAssignment(Rect(0,441,0,448));
      generator.MBFFsizing(placed_mbffs);
    }
  }
  

  for (auto ff : flip_flops) {
    ff->relocatedX = ff->position.x;
    ff->relocatedY = ff->position.y;
  }

  for (auto ff : flip_flops) {
    delete ff;
  }
  flip_flops.clear();
  return 0;
}