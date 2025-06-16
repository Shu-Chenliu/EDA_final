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
#include <random>  // For random device and engine

using namespace std;
int main() {
  vector<FF*> flip_flops = {
    new FF(1, 1, 10, 10, "ff0",
      { Pin{ Point(5, 5), 10.0, 2 } },
      { Pin{ Point(15, 15), 8.0, 3 } }
    ),
    new FF(1, 1, 12, 11, "ff1",
      { Pin{ Point(11, 10), 7.0, 1 } },
      { Pin{ Point(16, 16), 9.0, 2 } }
    ),
    new FF(1, 1, 11, 9,  "ff2", 
      { Pin{ Point(6, 6), 5.0, 1 } },
      { Pin{ Point(14, 14), 6.0, 2 } }
    ),
    new FF(1, 1, 9,  12, "ff3", 
      { Pin{ Point(4, 4), 3.0, 1 } },
      { Pin{ Point(13, 13), 4.0, 2 } }
    ),
    new FF(1, 1, 15, 15, "ff4",
      { Pin{ Point(10, 10), 12.0, 2 } },
      { Pin{ Point(20, 20), 11.0, 3 } }
    ),
    new FF(1, 1, 17, 16, "ff5",
      { Pin{ Point(14, 15), 10.0, 1 }, 
        Pin{ Point(18, 17), 9.0, 2 } },
      { Pin{ Point(22, 22), 8.0, 3 } }
    ),
    new FF(1, 1, 18, 19, "ff6",
      { Pin{ Point(13, 14), 8.0, 1 }, 
        Pin{ Point(17, 18), 9.0, 2 },
        Pin{ Point(20, 21), 7.0, 3 } },
      { Pin{ Point(21, 22), 7.0, 2 },
        Pin{ Point(24, 25), 6.0, 3 } }
    ),
    new FF(1, 1, 100, 100, "ff7", 
      { Pin{ Point(95, 95), 90.0, 2 }, 
        Pin{ Point(95, 93), 90.0, 2 } },
      { Pin{ Point(105, 105), 85.0, 3 } }
    ),
    new FF(1, 1, 102, 101, "ff8", 
      { Pin{ Point(97, 96), 88.0, 1 } },
      { Pin{ Point(106, 107), 84.0, 2 } }
    ),
    new FF(1, 12, 101, 99, "ff9", 
      { Pin{ Point(96, 94), 87.0, 1 } },
      { Pin{ Point(104, 103), 83.0, 2 } }
    ),
    new FF(1, 1, 99, 102, "ff10", 
      { Pin{ Point(95, 93), 86.0, 1 } },
      { Pin{ Point(103, 108), 82.0, 2 } }
    ),
    new FF(1, 1, 250, 250, "ff11", 
      { Pin{ Point(245, 245), 240.0, 2 } },
      { Pin{ Point(255, 255), 235.0, 3 }, 
        Pin{ Point(255, 253), 235.0, 2 },
        Pin{ Point(255, 252), 235.0, 2 } }
    ),
    new FF(1, 15, 251, 249, "ff12", 
      { Pin{ Point(246, 244), 238.0, 1 } },
      { Pin{ Point(256, 254), 234.0, 2 } }
    ),
    new FF(1, 17, 60, 305, "ff13", 
      { Pin{ Point(55, 300), 50.0, 2 } },
      { Pin{ Point(65, 310), 45.0, 3 } }
    ),
    new FF(1, 12, 62, 307, "ff14", 
      { Pin{ Point(57, 302), 48.0, 1 } },
      { Pin{ Point(67, 312), 43.0, 2 } }
    ),
    new FF(1, 1, 61, 303, "ff15", 
      { Pin{ Point(56, 301), 49.0, 1 } },
      { Pin{ Point(66, 308), 44.0, 2 } }
    ),
    new FF(1, 1, 59, 308, "ff16", 
      { Pin{ Point(54, 306), 51.0, 1 } },
      { Pin{ Point(64, 313), 46.0, 2 } }
    ),
    new FF(1, 14, 80, 405, "ff17", 
      { Pin{ Point(75, 400), 70.0, 2 } },
      { Pin{ Point(85, 410), 65.0, 3 } }
    ),
    new FF(1, 11, 82, 407, "ff18", 
      { Pin{ Point(77, 402), 68.0, 1 } },
      { Pin{ Point(87, 412), 63.0, 2 } }
    ),
    new FF(1, 13, 81, 403, "ff19", 
      { Pin{ Point(76, 401), 69.0, 1 } },
      { Pin{ Point(86, 408), 64.0, 2 } }
    ),
    new FF(1, 16, 79, 408, "ff20", 
      { Pin{ Point(74, 406), 71.0, 1 } },
      { Pin{ Point(84, 413), 66.0, 2 } }
    ),
    new FF(1, 11, 300, 300, "ff21", 
      { Pin{ Point(295, 295), 290.0, 2 } },
      { Pin{ Point(305, 305), 285.0, 3 } }
    ),
    new FF(1, 17, 302, 301, "ff22", 
      { Pin{ Point(297, 296), 288.0, 1 } },
      { Pin{ Point(306, 307), 284.0, 2 }, 
          Pin{ Point(306, 306), 284.0, 2 },
          Pin{ Point(306, 305), 284.0, 2 } }
    ),
    new FF(1, 4, 301, 299, "ff23", 
      { Pin{ Point(296, 294), 287.0, 1 } },
      { Pin{ Point(305, 304), 283.0, 2 } }
    ),
    new FF(1, 11, 299, 302, "ff24", 
      { Pin{ Point(295, 293), 286.0, 1 } },
      { Pin{ Point(304, 308), 282.0, 2 } }
    ),
    new FF(1, 13, 400, 400, "ff25", 
      { Pin{ Point(395, 395), 390.0, 2 } },
      { Pin{ Point(405, 405), 385.0, 3 } }
    ),
    new FF(1, 2, 402, 401, "ff26", 
      { Pin{ Point(397, 396), 388.0, 1 } },
      { Pin{ Point(406, 407), 384.0, 2 }, 
        Pin{ Point(406, 406), 384.0, 2 },
        Pin{ Point(406, 405), 384.0, 2 } }
    ),
    new FF(1, 7, 401, 399, "ff27", 
      { Pin{ Point(396, 394), 387.0, 1 }, 
        Pin{ Point(398, 398), 386.0, 1 }, 
        Pin{ Point(400, 400), 385.0, 1 } },
      { Pin{ Point(405, 404), 383.0, 2 } }
    ),
    new FF(1, 8, 399, 402, "ff28", 
      { Pin{ Point(395, 393), 386.0, 1 } },
      { Pin{ Point(404, 408), 382.0, 2 } }
    )
  };
  for(size_t i=0;i<flip_flops.size();i++){
    random_device rd;
    mt19937 gen(rd());
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
      flip_flops[i]->next.push_back(flip_flops[indices[j]]);
      cout<<indices[j]<<" ";
    }
    cout<<endl;
  }
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
  int SIZE_LIMIT = flip_flops.size() / 3 ; // Example size limit for clusters
  int MAX_ITER = flip_flops.size() * 2; // Example maximum iterations
  int DISP_LIMIT = (right - left + bottom - top) / 3;
  kmean kmean(SIZE_LIMIT,MAX_ITER,DISP_LIMIT);
  // update flip flops position
  

  vector<Cluster> clusters=kmean.kmeansWeighted(flip_flops);
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