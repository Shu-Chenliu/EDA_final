#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <set>
#include <random>  // For random device and engine
#include "Board.h"
#include "Coor.h"
#include "Rect.h"
#include "Pin.h"
#include "FF.h"
#include "MBFFgeneration.h"
#include "kmean.h"
#include "MST.h"


using namespace std;
int main() {
  Board board(Rect(0,441,0,448));

  random_device rd;
  mt19937 gen(rd());
  vector<FF*> flip_flops;

  // Step 1: 建立 FF 物件（先不加 fanins/fanouts）
  flip_flops.push_back(new FF("ff0","",10,10,1,1));
  flip_flops.push_back(new FF("ff1","",20,10,1,1));
  flip_flops.push_back(new FF("ff2","",13,10,1,1));
  flip_flops.push_back(new FF("ff3","",40,62,1,1));
  flip_flops.push_back(new FF("ff4","",50,95,1,1));
  flip_flops.push_back(new FF("ff5","",60,1,1,1));
  flip_flops.push_back(new FF("ff6","",7,205,1,1));
  flip_flops.push_back(new FF("ff7","",94,128,1,1));
  flip_flops.push_back(new FF("ff8","",100,100,1,1));
  flip_flops.push_back(new FF("ff9","",120,120,1,1));
  flip_flops.push_back(new FF("ff10","",130,130,1,1));
  flip_flops.push_back(new FF("ff11","",140,140,1,1));
  flip_flops.push_back(new FF("ff12","",12, 94,1,1));
  flip_flops.push_back(new FF("ff13","",15, 95,1,1));
  flip_flops.push_back(new FF("ff14","",204, 80,1,1));
  flip_flops.push_back(new FF("ff15","",205, 190,1,1));
  flip_flops.push_back(new FF("ff16","",300, 200,1,1));
  flip_flops.push_back(new FF("ff17","",400, 300,1,1));
  flip_flops.push_back(new FF("ff18","",205, 44, 1,1));
  flip_flops.push_back(new FF("ff19","",370, 400,1,1));
  flip_flops.push_back(new FF("ff20","",500, 53, 1,1));
  flip_flops.push_back(new FF("ff21","",100, 41, 1,1));
  flip_flops.push_back(new FF("ff22","",200, 100, 1,1));
  flip_flops.push_back(new FF("ff23","",300, 200, 1,1));
  flip_flops.push_back(new FF("ff24","",400, 300, 1,1));
  flip_flops.push_back(new FF("ff25","",500, 400, 1,1));
  flip_flops.push_back(new FF("ff26","",67, 194, 1,1));
  flip_flops.push_back(new FF("ff27","",80, 200, 1,1));
  flip_flops.push_back(new FF("ff28","",90, 210, 1,1));
  flip_flops.push_back(new FF("ff29","",100, 220, 1,1));
  flip_flops.push_back(new FF("ff30","",110, 230, 1,1));
  // cout << "Total flip flops: " << flip_flops.size() << endl;
  // for (size_t i = 0; i < flip_flops.size(); ++i) {
  //   cout << "FF " << i << ": " << flip_flops[i]->getName() << " at (" 
  //        << flip_flops[i]->getX() << ", " << flip_flops[i]->getY() << ")" << endl;
  // }


  vector<Pin> pins = { Pin("pin1", "", flip_flops[0]) };
  flip_flops[0]->addPins(pins);
  
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
      flip_flops[i]->getNext().push_back(indices[j]);
      cout<<indices[j]<<" ";
    }
    cout << "getNext: " << flip_flops[i]->getNext().size() << endl;
 
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
  cout << "FF size: " << flip_flops.size() << endl;
  for (size_t i = 0; i < flip_flops.size(); ++i) {
    for (size_t j = 0; j < flip_flops[i]->getNext().size(); ++j) {
      edges.push_back(Edge(
        i,
        flip_flops[i]->getNext()[j],
        (int)abs(flip_flops[i]->getX() - flip_flops[flip_flops[i]->getNext()[j]]->getX()) +
        abs(flip_flops[i]->getY() - flip_flops[flip_flops[i]->getNext()[j]]->getY())
      )); // Manhattan distance

    
      cout << "Edge from " << flip_flops[i]->getName() << " to " 
           << flip_flops[flip_flops[i]->getNext()[j]]->getName() 
           << " with weight "
           << abs(flip_flops[i]->getX() - flip_flops[flip_flops[i]->getNext()[j]]->getX()) +
           abs(flip_flops[i]->getY() - flip_flops[flip_flops[i]->getNext()[j]]->getY())
           << endl;
      
    }
  }
  // do MST
  MST mst_before(edges, (int)flip_flops.size());
  total_wire_length = mst_before.MinimumSpanningTreeCost();
  cout << "Initial MST wire length: " << total_wire_length << endl;


  int left = 0, right = 0, top = 0, bottom = 0;
  for (auto ff : flip_flops) {
    if (ff->getX() < left) left = ff->getX();
    if (ff->getX() > right) right = ff->getX();
    if (ff->getY() < top) top = ff->getY();
    if (ff->getY() > bottom) bottom = ff->getY();
  }
  size_t size=flip_flops.size();
  for(size_t j=0;j<1;j++){
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
      for (size_t j = 0; j < flip_flops[i]->getNext().size(); ++j) {
        edges.push_back(Edge(
          i,
          flip_flops[i]->getNext()[j],
          (int)abs(flip_flops[i]->getRelocateCoor().getX() - flip_flops[flip_flops[i]->getNext()[j]]->getRelocateCoor().getX()) +
          abs(flip_flops[i]->getRelocateCoor().getY() - flip_flops[flip_flops[i]->getNext()[j]]->getRelocateCoor().getY())
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
      vector<MBFF> placed_mbffs=generator.locationAssignment(board);
      generator.MBFFsizing(placed_mbffs);
    }
  }
  

  for (auto ff : flip_flops) {
    ff->setRelocateCoor(Coor(ff->getX(),ff->getY()));
  }

  for (auto ff : flip_flops) {
    delete ff;
  }
  flip_flops.clear();
  return 0;
}