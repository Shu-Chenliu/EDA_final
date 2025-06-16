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
  flip_flops.push_back(new FF(1, 1, 20, 20, "ff7"));
  flip_flops.push_back(new FF(1, 1, 22, 21, "ff8"));
  flip_flops.push_back(new FF(1, 1, 240, 193, "ff9"));
  flip_flops.push_back(new FF(1, 1, 200, 150, "ff10"));
  flip_flops.push_back(new FF(1, 1, 180, 170, "ff11"));
  flip_flops.push_back(new FF(1, 1, 160, 180, "ff12"));
  flip_flops.push_back(new FF(1, 1, 140, 190, "ff13"));
  flip_flops.push_back(new FF(1, 1, 120, 200, "ff14"));
  flip_flops.push_back(new FF(1, 1, 480, 210, "ff15"));
  flip_flops.push_back(new FF(1, 1, 500, 220, "ff16"));
  flip_flops.push_back(new FF(1, 1, 490, 2, "ff17"));
  flip_flops.push_back(new FF(1, 1, 480, 3, "ff18"));
  flip_flops.push_back(new FF(1, 1, 270, 45, "ff19"));
  flip_flops.push_back(new FF(1, 1, 300, 50, "ff20"));
  flip_flops.push_back(new FF(1, 1, 350, 60, "ff21"));
  flip_flops.push_back(new FF(1, 1, 400, 70, "ff22"));
  flip_flops.push_back(new FF(1, 1, 450, 80, "ff23"));
  flip_flops.push_back(new FF(1, 1, 500, 90, "ff24"));
  flip_flops.push_back(new FF(1, 1, 550, 100, "ff25"));
  flip_flops.push_back(new FF(1, 1, 290, 114, "ff26"));
  flip_flops.push_back(new FF(1, 1, 310, 129, "ff27"));
  flip_flops.push_back(new FF(1, 1, 330, 140, "ff28"));

  // Step 2: 為每個 FF 補上 fanins 與 fanouts 的 Pin，並填入 ff 指標
  flip_flops[0]->fanins = {
      Pin{ Point(0, 5), flip_flops[0], 10.0, 2 }
  };
  flip_flops[0]->fanouts = {
      Pin{ Point(15, 5), flip_flops[0], 8.0, 3 }
  };

  flip_flops[1]->fanins = {
      Pin{ Point(11, 10), flip_flops[1], 7.0, 1 }
  };
  flip_flops[1]->fanouts = {
      Pin{ Point(16, 6), flip_flops[1], 9.0, 2 }
  };

  flip_flops[2]->fanins = {
      Pin{ Point(6, 8), flip_flops[2], 5.0, 1 }
  };
  flip_flops[2]->fanouts = {
      Pin{ Point(4, 14), flip_flops[2], 6.0, 2 }
  };

  flip_flops[3]->fanins = {
      Pin{ Point(4, 4), flip_flops[3], 3.0, 1 }
  };
  flip_flops[3]->fanouts = {
      Pin{ Point(12, 13), flip_flops[3], 4.0, 2 }
  };

  flip_flops[4]->fanins = {
      Pin{ Point(10, 1), flip_flops[4], 12.0, 2 }
  };
  flip_flops[4]->fanouts = {
      Pin{ Point(2, 2), flip_flops[4], 11.0, 3 }
  };

  flip_flops[5]->fanins = {
      Pin{ Point(14, 15), flip_flops[5], 10.0, 1 },
      Pin{ Point(18, 17), flip_flops[5], 9.0, 2 }
  };
  flip_flops[5]->fanouts = {
      Pin{ Point(12, 12), flip_flops[5], 8.0, 3 }
  };

  flip_flops[6]->fanins = {
      Pin{ Point(3, 1), flip_flops[6], 8.0, 1 },
      Pin{ Point(7, 1), flip_flops[6], 9.0, 2 },
      Pin{ Point(2, 2), flip_flops[6], 7.0, 3 }
  };
  flip_flops[6]->fanouts = {
      Pin{ Point(11, 2), flip_flops[6], 7.0, 2 },
      Pin{ Point(4, 5), flip_flops[6], 6.0, 3 }
  };
  flip_flops[7]->fanins = {
      Pin{ Point(9, 2), flip_flops[7], 6.0, 1 },
      Pin{ Point(2, 4), flip_flops[7], 5.0, 2 }
  };
  flip_flops[7]->fanouts = {
      Pin{ Point(6, 7), flip_flops[7], 4.0, 3 }
  };
  flip_flops[8]->fanins = {
      Pin{ Point(5, 6), flip_flops[8], 3.0, 1 }
  };
  flip_flops[8]->fanouts = {
      Pin{ Point(8, 9), flip_flops[8], 2.0, 2 }
  };
  flip_flops[9]->fanins = {
      Pin{ Point(7, 8), flip_flops[9], 1.0, 1 }
  };
  flip_flops[9]->fanouts = {
      Pin{ Point(3, 3), flip_flops[9], 0.0, 2 }
  };
  flip_flops[10]->fanins = {
      Pin{ Point(9, 3), flip_flops[10], 1.0, 1 }
  };
  flip_flops[10]->fanouts = {
      Pin{ Point(2, 13), flip_flops[10], 2.0, 2 }
  };
  flip_flops[11]->fanins = {
      Pin{ Point(3, 3), flip_flops[11], 3.0, 1 }
  };
  flip_flops[11]->fanouts = {
      Pin{ Point(3, 5), flip_flops[11], 4.0, 2 }
  };
  flip_flops[12]->fanins = {
      Pin{ Point(13, 4), flip_flops[12], 5.0, 1 },
      Pin{ Point(16, 7), flip_flops[12], 6.0, 2 },
      Pin{ Point(18, 19), flip_flops[12], 7.0, 3 }
  };
  flip_flops[12]->fanouts = {
      Pin{ Point(4, 4), flip_flops[12], 8.0, 2 },
      Pin{ Point(2, 4), flip_flops[12], 9.0, 3 }
  };
  flip_flops[13]->fanins = {
      Pin{ Point(5, 6), flip_flops[13], 10.0, 1 },
      Pin{ Point(4, 4), flip_flops[13], 11.0, 2 }
  };
  flip_flops[13]->fanouts = {
      Pin{ Point(14, 15), flip_flops[13], 12.0, 3 }
  };
  flip_flops[14]->fanins = {
      Pin{ Point(7, 8), flip_flops[14], 13.0, 1 },
      Pin{ Point(42, 13), flip_flops[14], 14.0, 2 }
  };
  flip_flops[14]->fanouts = {
      Pin{ Point(16, 47), flip_flops[14], 15.0, 3 }
  };
  flip_flops[15]->fanins = {
      Pin{ Point(9, 4), flip_flops[15], 16.0, 1 },
      Pin{ Point(4, 15), flip_flops[15], 17.0, 2 }
  };
  flip_flops[15]->fanouts = {
      Pin{ Point(18, 49), flip_flops[15], 18.0, 3 }
  };
  flip_flops[16]->fanins = {
      Pin{ Point(11, 12), flip_flops[16], 19.0, 1 },
      Pin{ Point(16, 17), flip_flops[16], 20.0, 2 }
  };
  flip_flops[16]->fanouts = {
      Pin{ Point(10, 5), flip_flops[16], 21.0, 3 }
  };
  flip_flops[17]->fanins = {
      Pin{ Point(3, 4), flip_flops[17], 22.0, 1 },
      Pin{ Point(8, 9), flip_flops[17], 23.0, 2 }
  };
  flip_flops[17]->fanouts = {
      Pin{ Point(2, 3), flip_flops[17], 24.0, 3 },
      Pin{ Point(4, 5), flip_flops[17], 25.0, 4 },
      Pin{ Point(6, 7), flip_flops[17], 26.0, 5 }
  };
  flip_flops[18]->fanins = {
      Pin{ Point(4, 6), flip_flops[18], 27.0, 1 },
      Pin{ Point(5, 1), flip_flops[18], 28.0, 2 }
  };
  flip_flops[18]->fanouts = {
      Pin{ Point(18, 5), flip_flops[18], 29.0, 3 }
  };
  flip_flops[19]->fanins = {
      Pin{ Point(1, 18), flip_flops[19], 30.0, 1 },
      Pin{ Point(51, 13), flip_flops[19], 31.0, 2 }
  };
  flip_flops[19]->fanouts = {
      Pin{ Point(10, 11), flip_flops[19], 32.0, 3 }
  }; 
  flip_flops[20]->fanins = {
      Pin{ Point(19, 10), flip_flops[20], 33.0, 1 },
      Pin{ Point(14, 15), flip_flops[20], 34.0, 2 }
  };
  flip_flops[20]->fanouts = {
      Pin{ Point(12, 13), flip_flops[20], 35.0, 3 }
  };
  flip_flops[21]->fanins = {
      Pin{ Point(1, 5), flip_flops[21], 36.0, 1 },
      Pin{ Point(6, 7), flip_flops[21], 37.0, 2 }
  };
  flip_flops[21]->fanouts = {
      Pin{ Point(6, 5), flip_flops[21], 38.0, 3 }
  };
  flip_flops[22]->fanins = {
      Pin{ Point(3, 4), flip_flops[22], 39.0, 1 },
      Pin{ Point(5, 9), flip_flops[22], 40.0, 2 }
  };
  flip_flops[22]->fanouts = {
      Pin{ Point(6, 7), flip_flops[22], 41.0, 3 }
  };
  flip_flops[23]->fanins = {
      Pin{ Point(55, 56), flip_flops[23], 42.0, 1 },
      Pin{ Point(60, 61), flip_flops[23], 43.0, 2 }
  };
  flip_flops[23]->fanouts = {
      Pin{ Point(8, 9), flip_flops[23], 44.0, 3 }
  };
  flip_flops[24]->fanins = {
      Pin{ Point(5, 15), flip_flops[24], 45.0, 1 },
      Pin{ Point(2, 6), flip_flops[24], 46.0, 2 }
  };
  flip_flops[24]->fanouts = {
      Pin{ Point(17, 17), flip_flops[24], 47.0, 3 }
  };
  flip_flops[25]->fanins = {
      Pin{ Point(9, 6), flip_flops[25], 48.0, 1 },
      Pin{ Point(4, 6), flip_flops[25], 49.0, 2 }
  };
  flip_flops[25]->fanouts = {
      Pin{ Point(2, 3), flip_flops[25], 50.0, 3 }
  };
  flip_flops[26]->fanins = {
      Pin{ Point(1, 2), flip_flops[26], 51.0, 1 },
      Pin{ Point(6, 7), flip_flops[26], 52.0, 2 }
  };
  flip_flops[26]->fanouts = {
      Pin{ Point(74, 75), flip_flops[26], 53.0, 3 }
  };
  flip_flops[27]->fanins = {
      Pin{ Point(13, 14), flip_flops[27], 54.0, 1 },
      Pin{ Point(2, 19), flip_flops[27], 55.0, 2 }
  };
  flip_flops[27]->fanouts = {
      Pin{ Point(26, 7), flip_flops[27], 56.0, 3 }
  };
  flip_flops[28]->fanins = {
      Pin{ Point(25, 6), flip_flops[28], 57.0, 1 },
      Pin{ Point(20, 21), flip_flops[28], 58.0, 2 }
  };
  flip_flops[28]->fanouts = {
      Pin{ Point(28, 29), flip_flops[28], 59.0, 3 }
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