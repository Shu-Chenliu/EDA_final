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

  // cout << "Total flip flops: " << flip_flops.size() << endl;
  // for (size_t i = 0; i < flip_flops.size(); ++i) {
  //   cout << "FF " << i << ": " << flip_flops[i]->getName() << " at (" 
  //        << flip_flops[i]->getX() << ", " << flip_flops[i]->getY() << ")" << endl;
  // }


  vector<Pin> pins = { Pin("pin1", "", flip_flops[0]) };
  flip_flops[0]->addPins(pins);
  vector<Pin> pins2 = { Pin("pin2", "", flip_flops[1]), 
                        Pin("pin3", "", flip_flops[1]),
                        Pin("pin4", "", flip_flops[1]) };
  flip_flops[1]->addPins(pins2);
  vector<Pin> pins3 = { Pin("pin5", "", flip_flops[2]), 
                        Pin("pin6", "", flip_flops[2]) };
  flip_flops[2]->addPins(pins3);
  vector<Pin> pins4 = { Pin("pin7", "", flip_flops[3])};
  flip_flops[3]->addPins(pins4);
  vector<Pin> pins5 = { Pin("pin8", "", flip_flops[4])};
  flip_flops[4]->addPins(pins5);
  vector<Pin> pins6 = { Pin("pin9", "", flip_flops[5]), 
                        Pin("pin10", "", flip_flops[5]) };
  flip_flops[5]->addPins(pins6);
  vector<Pin> pins7 = { Pin("pin11", "", flip_flops[6]), 
                        Pin("pin12", "", flip_flops[6]) };
  flip_flops[6]->addPins(pins7);
  vector<Pin> pins8 = { Pin("pin13", "", flip_flops[7]), 
                        Pin("pin14", "", flip_flops[7]) };
  flip_flops[7]->addPins(pins8);
  vector<Pin> pins9 = { Pin("pin15", "", flip_flops[8]), 
                        Pin("pin16", "", flip_flops[8]) };
  flip_flops[8]->addPins(pins9);
  vector<Pin> pins10 = { Pin("pin17", "", flip_flops[9]), 
                        Pin("pin18", "", flip_flops[9]) };
  flip_flops[9]->addPins(pins10);
  vector<Pin> pins11 = { Pin("pin19", "", flip_flops[10]), 
                        Pin("pin20", "", flip_flops[10]) };
  flip_flops[10]->addPins(pins11);
  vector<Pin> pins12 = { Pin("pin21", "", flip_flops[11]), 
                        Pin("pin22", "", flip_flops[11]),
                        Pin("pin23", "", flip_flops[11]),
                        Pin("pin24", "", flip_flops[11]) };
  flip_flops[11]->addPins(pins12);
  vector<Pin> pins13 = { Pin("pin25", "", flip_flops[12]), 
                        Pin("pin26", "", flip_flops[12]) };
  flip_flops[12]->addPins(pins13);
  vector<Pin> pins14 = { Pin("pin27", "", flip_flops[13])}; 
  flip_flops[13]->addPins(pins14);
  vector<Pin> pins15 = { Pin("pin28", "", flip_flops[14])}; 
  flip_flops[14]->addPins(pins15);
  vector<Pin> pins16 = { Pin("pin29", "", flip_flops[15]), 
                        Pin("pin30", "", flip_flops[15]) };
  flip_flops[15]->addPins(pins16);
  vector<Pin> pins17 = { Pin("pin31", "", flip_flops[16]), 
                        Pin("pin32", "", flip_flops[16]) };
  flip_flops[16]->addPins(pins17);
  vector<Pin> pins18 = { Pin("pin33", "", flip_flops[17]), 
                        Pin("pin34", "", flip_flops[17]) };
  flip_flops[17]->addPins(pins18);
  vector<Pin> pins19 = { Pin("pin35", "", flip_flops[18]), 
                        Pin("pin36", "", flip_flops[18]) };
  flip_flops[18]->addPins(pins19);
  vector<Pin> pins20 = { Pin("pin37", "", flip_flops[19]), 
                        Pin("pin38", "", flip_flops[19]) };
  flip_flops[19]->addPins(pins20);
  vector<Pin> pins21 = { Pin("pin39", "", flip_flops[20]), 
                        Pin("pin40", "", flip_flops[20]) };
  flip_flops[20]->addPins(pins21);
  vector<Pin> pins22 = { Pin("pin41", "", flip_flops[21]), 
                        Pin("pin42", "", flip_flops[21]) };
  flip_flops[21]->addPins(pins22);
  vector<Pin> pins23 = { Pin("pin43", "", flip_flops[22]), 
                        Pin("pin44", "", flip_flops[22]) }; 
  flip_flops[22]->addPins(pins23);
  vector<Pin> pins24 = { Pin("pin45", "", flip_flops[23]), 
                        Pin("pin46", "", flip_flops[23]) };
  flip_flops[23]->addPins(pins24);
  vector<Pin> pins25 = { Pin("pin47", "", flip_flops[24]), 
                        Pin("pin48", "", flip_flops[24]),
                        Pin("pin49", "", flip_flops[24]) };
  flip_flops[24]->addPins(pins25);
  vector<Pin> pins26 = { Pin("pin50", "", flip_flops[25]), 
                        Pin("pin51", "", flip_flops[25]) };
  flip_flops[25]->addPins(pins26);
  vector<Pin> pins27 = { Pin("pin52", "", flip_flops[26]), 
                        Pin("pin53", "", flip_flops[26]) };
  flip_flops[26]->addPins(pins27);
  vector<Pin> pins28 = { Pin("pin54", "", flip_flops[27]), 
                        Pin("pin55", "", flip_flops[27]) };
  flip_flops[27]->addPins(pins28);
  vector<Pin> pins29 = { Pin("pin56", "", flip_flops[28]), 
                        Pin("pin57", "", flip_flops[28]) };
  flip_flops[28]->addPins(pins29);
  
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
      flip_flops[i]->addNext(indices[j]);
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