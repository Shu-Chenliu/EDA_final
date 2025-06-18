#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <set>
#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include <typeinfo>
#include <random>  // For random device and engine
#include "Board.h"
#include "Coor.h"
#include "Rect.h"
#include "Pin.h"
#include "FF.h"
#include "MBFFgeneration.h"
#include "kmean.h"
#include "MST.h"
#include "legalization.h"
//TODO: write bin class

using namespace std;

vector<string> split(const string& s){
  istringstream iss(s);
  string ss;
  vector<string> str;
  while (iss >> ss){
    str.push_back(ss);
  }
  return str;
}

void readFile(string file){
  ifstream f(file);

  if (!f.is_open()) {
    cerr << "Fail to open" << endl;
    return;
  }

  string s;
  vector<string> str;
  while (getline(f, s)){
    cout << "/" << s << "/" << endl;
    str = split(s);
    for (int i=0; i<(int)str.size(); i++){
      cout << " ~" << str[i] << "~ " << endl;
    }
  }
  f.close();
  return;
} 
vector<Bin> generateBins(Board board) {
	vector<Bin> bins;
	int x_bins = (board.getW()) / board.getBinW();
	int y_bins = (board.getH()) / board.getBinH();

	for (int i = 0; i < x_bins; ++i) {
		for (int j = 0; j < y_bins; ++j) {
			Rect area(board.getBinW(),board.getBinH(),board.getSize().getX()+i*board.getBinW(),board.getSize().getY()+j * board.getBinH());
			bins.push_back({i,j,area});
		}
	}
	return bins;
}
int main() {
  Board board;
  // string file = "../testcase1/testcase1";
  // board.readWeight(file);
  // board.readDef(file);
  // board.readV(file);
  // board.forMatplotlib(file);
  // vector<Bin> bins=generateBins(board);
  // cout<<"generate"<<bins.size()<<"bins"<<endl;
  random_device rd;
  mt19937 gen(rd());
  board.setSize(1000, 1000);
  board.setBinW(100);
  board.setBinH(100);
  board.setBinShift(0, 0);
  board.setAlpha(1);
  board.setBeta(1);
  board.setGamma(1);
  board.setTNS(0);
  board.setTPO(0);
  board.setArea(1);
 
  vector<Bin> bins = generateBins(board);

  // get FFs from board
  vector<pair<Cell*, int>> board_FFs = board.getFFs();
   
  
  vector<FF*> flip_flops;
  // FFs -> flip_flops
  for (int i = 0; i < board_FFs.size(); ++i) {
    Cell* cell = board_FFs[i].first;
    int bit = board_FFs[i].second;
    FF* ff = new FF(cell->getName(), cell->getModel(), cell->getX(), cell->getY(), cell->getW(), cell->getH());
    ff->setBit(bit);
    ff->setClk(1);  // Assuming clk is always 1 for simplicity
    ff->setScan(0); // Assuming scan is always 0 for simplicity
    ff->setRelocateCoor(Coor(cell->getX(), cell->getY()));
    flip_flops.push_back(ff);
    
  }




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


  vector<Pin> pins = { Pin("pin1", "", flip_flops[0],2,2) };
  flip_flops[0]->addPins(pins);
  vector<Pin> pins2 = { Pin("pin2", "", flip_flops[1],2,2), 
                        Pin("pin3", "", flip_flops[1],2,2),
                        Pin("pin4", "", flip_flops[1],2,2) };
  flip_flops[1]->addPins(pins2);
  vector<Pin> pins3 = { Pin("pin5", "", flip_flops[2],2,2), 
                        Pin("pin6", "", flip_flops[2],2,2) };
  flip_flops[2]->addPins(pins3);
  vector<Pin> pins4 = { Pin("pin7", "", flip_flops[3],2,2)};
  flip_flops[3]->addPins(pins4);
  vector<Pin> pins5 = { Pin("pin8", "", flip_flops[4],2,2)};
  flip_flops[4]->addPins(pins5);
  vector<Pin> pins6 = { Pin("pin9", "", flip_flops[5],2,2), 
                        Pin("pin10", "", flip_flops[5],2,2) };
  flip_flops[5]->addPins(pins6);
  vector<Pin> pins7 = { Pin("pin11", "", flip_flops[6],2,2), 
                        Pin("pin12", "", flip_flops[6],2,2) };
  flip_flops[6]->addPins(pins7);
  vector<Pin> pins8 = { Pin("pin13", "", flip_flops[7],2,2), 
                        Pin("pin14", "", flip_flops[7],2,2) };
  flip_flops[7]->addPins(pins8);
  vector<Pin> pins9 = { Pin("pin15", "", flip_flops[8],2,2), 
                        Pin("pin16", "", flip_flops[8],2,2) };
  flip_flops[8]->addPins(pins9);
  vector<Pin> pins10 = { Pin("pin17", "", flip_flops[9],2,2), 
                        Pin("pin18", "", flip_flops[9],2,2) };
  flip_flops[9]->addPins(pins10);
  vector<Pin> pins11 = { Pin("pin19", "", flip_flops[10],2,2), 
                        Pin("pin20", "", flip_flops[10],2,2) };
  flip_flops[10]->addPins(pins11);
  vector<Pin> pins12 = { Pin("pin21", "", flip_flops[11],2,2), 
                        Pin("pin22", "", flip_flops[11],2,2),
                        Pin("pin23", "", flip_flops[11],2,2),
                        Pin("pin24", "", flip_flops[11],2,2) };
  flip_flops[11]->addPins(pins12);
  vector<Pin> pins13 = { Pin("pin25", "", flip_flops[12],2,2), 
                        Pin("pin26", "", flip_flops[12],2,2) };
  flip_flops[12]->addPins(pins13);
  vector<Pin> pins14 = { Pin("pin27", "", flip_flops[13],2,2)}; 
  flip_flops[13]->addPins(pins14);
  vector<Pin> pins15 = { Pin("pin28", "", flip_flops[14],2,2)}; 
  flip_flops[14]->addPins(pins15);
  vector<Pin> pins16 = { Pin("pin29", "", flip_flops[15],2,2), 
                        Pin("pin30", "", flip_flops[15],2,2) };
  flip_flops[15]->addPins(pins16);
  vector<Pin> pins17 = { Pin("pin31", "", flip_flops[16],2,2), 
                        Pin("pin32", "", flip_flops[16],2,2) };
  flip_flops[16]->addPins(pins17);
  vector<Pin> pins18 = { Pin("pin33", "", flip_flops[17],2,2), 
                        Pin("pin34", "", flip_flops[17],2,2) };
  flip_flops[17]->addPins(pins18);
  vector<Pin> pins19 = { Pin("pin35", "", flip_flops[18],2,2), 
                        Pin("pin36", "", flip_flops[18],2,2) };
  flip_flops[18]->addPins(pins19);
  vector<Pin> pins20 = { Pin("pin37", "", flip_flops[19],2,2), 
                        Pin("pin38", "", flip_flops[19],2,2) };
  flip_flops[19]->addPins(pins20);
  vector<Pin> pins21 = { Pin("pin39", "", flip_flops[20],2,2), 
                        Pin("pin40", "", flip_flops[20],2,2) };
  flip_flops[20]->addPins(pins21);
  vector<Pin> pins22 = { Pin("pin41", "", flip_flops[21],2,2), 
                        Pin("pin42", "", flip_flops[21],2,2) };
  flip_flops[21]->addPins(pins22);
  vector<Pin> pins23 = { Pin("pin43", "", flip_flops[22],2,2), 
                        Pin("pin44", "", flip_flops[22],2,2) }; 
  flip_flops[22]->addPins(pins23);
  vector<Pin> pins24 = { Pin("pin45", "", flip_flops[23],2,2), 
                        Pin("pin46", "", flip_flops[23],2,2) };
  flip_flops[23]->addPins(pins24);
  vector<Pin> pins25 = { Pin("pin47", "", flip_flops[24],2,2), 
                        Pin("pin48", "", flip_flops[24],2,2),
                        Pin("pin49", "", flip_flops[24],2,2) };
  flip_flops[24]->addPins(pins25);
  vector<Pin> pins26 = { Pin("pin50", "", flip_flops[25],2,2), 
                        Pin("pin51", "", flip_flops[25],2,2) };
  flip_flops[25]->addPins(pins26);
  vector<Pin> pins27 = { Pin("pin52", "", flip_flops[26],2,2), 
                        Pin("pin53", "", flip_flops[26],2,2) };
  flip_flops[26]->addPins(pins27);
  vector<Pin> pins28 = { Pin("pin54", "", flip_flops[27],2,2), 
                        Pin("pin55", "", flip_flops[27],2,2) };
  flip_flops[27]->addPins(pins28);
  vector<Pin> pins29 = { Pin("pin56", "", flip_flops[28],2,2), 
                        Pin("pin57", "", flip_flops[28],2,2) };
  flip_flops[28]->addPins(pins29);
  Legalization legalize;
  legalize.legalizePlacing(flip_flops,bins,board);
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
  //TODO: stop when 2 local minimum occur
  int minCost = numeric_limits<int>::max();
  int currentCost = 0;
  int beforeCost = 0;
  int state = 0; // 1: increase, -1: decrease, 0: no change
  bool local_minimum_occur = false;
  int KmeanIteration = 10;
  vector<int> MST_costs;



  for(size_t j=0;j<KmeanIteration;j++){
    
    beforeCost = currentCost;
    
    uniform_int_distribution<> num_size((int)flip_flops.size() / 3, (int)flip_flops.size()*2 / 3);
    int SIZE_LIMIT = num_size(gen) ; // Example size limit for clusters
    int MAX_ITER = flip_flops.size() * 2; // Example maximum iterations
    uniform_int_distribution<> num_disp((int)(right - left + bottom - top) / 3, (int)(right - left + bottom - top) *2/ 3);
    int DISP_LIMIT = num_disp(gen);
    kmean kmean(SIZE_LIMIT,MAX_ITER,DISP_LIMIT);
    vector<Cluster> clusters=kmean.kmeansWeighted(flip_flops);
    cout << "K-means clustering completed with " << clusters.size() << " clusters." << endl;
    edges.clear();
    for (size_t ii = 0; ii < flip_flops.size(); ++ii) {
      for (size_t jj = 0; jj < flip_flops[ii]->getNext().size(); ++jj) {
        edges.push_back(Edge(
          ii,
          flip_flops[ii]->getNext()[jj],
          (int)abs(flip_flops[ii]->getRelocateCoor().getX() - flip_flops[flip_flops[ii]->getNext()[jj]]->getRelocateCoor().getX()) +
          abs(flip_flops[ii]->getRelocateCoor().getY() - flip_flops[flip_flops[ii]->getNext()[jj]]->getRelocateCoor().getY())
        )); // Manhattan distance

        // cout << "Edge from " << ii << " (" << flip_flops[ii]->getName() << ") to " 
        //      << flip_flops[ii]->getNext()[jj] << " (" << flip_flops[flip_flops[ii]->getNext()[jj]]->getName()
        //      << ") with weight "
        //      << abs(flip_flops[ii]->getRelocateCoor().getX() - flip_flops[flip_flops[ii]->getNext()[jj]]->getRelocateCoor().getX()) +
        //      abs(flip_flops[ii]->getRelocateCoor().getY() - flip_flops[flip_flops[ii]->getNext()[jj]]->getRelocateCoor().getY())
        //      << endl;
             
      }
    }

    // do MST
    MST mst_after(edges, (int)flip_flops.size());
    currentCost = mst_after.MinimumSpanningTreeCost();
    MST_costs.push_back(currentCost);
    cout << "MST wire length after k-means: " << currentCost << endl;

    srand(time(0));
    for(size_t i=0;i<1;i++){
      vector<FF*> flipflop=clusters[i].flip_flops;
      int maxDrivingStrength = 4;
      double beta = 0.95;
      MBFFgeneration generator(flipflop, maxDrivingStrength, beta);
      // vector<set<string>> mbff_result = generator.generateMBFF();
      vector<MBFF> placed_mbffs=generator.locationAssignment(bins);
      generator.MBFFsizing(placed_mbffs);
    }


    // estimate cost
    if (j == 0) {
      minCost = currentCost;
      beforeCost = currentCost;
      cout << "============ Initial cost: " << minCost << " =============" << endl;
    } else {
      cout << "============ Current cost: " << currentCost << ", Previous cost: " << beforeCost << " ============="<< endl;
      if (currentCost < beforeCost && state == 1) {
        state = -1; // Decrease
      } else if (currentCost > beforeCost && state == -1) {
        state = 1; // Increase
        if (local_minimum_occur){
          minCost = min(minCost, beforeCost);
          cout << "Iteration " << j << ": Local minimum occurred, stopping optimization." << endl;
          cout << "Cost: " << minCost << endl;
          break; // Stop the loop

        }else{
          local_minimum_occur = true;
          minCost = min(minCost, beforeCost);
          cout << "Iteration " << j << ": Local minimum not occurred, continue optimization." << endl;
          cout << "Minimum cost so far: " << minCost << endl;
        }
      } else if (currentCost < beforeCost) {
        state = -1; // Increase
        local_minimum_occur = false;
      } else if (currentCost > beforeCost) {
        state = 1; // Decrease
      }   
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