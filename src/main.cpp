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
  string file = "../testcase1/testcase1";
  board.readWeight(file);
  board.readDef(file);
  board.readV(file);
  board.forMatplotlib(file);
  vector<Bin> bins=generateBins(board);
  Netlist netlist=board.getNetList();
  random_device rd;
  mt19937 gen(rd());
  float exactTNS=board.getTNS();
  float exactPower=board.getTPO();
  float exactArea=board.getArea();
  vector<pair<Cell*, int>> board_FFs = board.getFFs();
  unordered_map<string,pair<FF*,int>> FFnameMap;
  float alpha=board.getAlpha();
  float beta=board.getBeta();
  float gamma=board.getGamma();
  vector<FF*> flip_flops;
  // FFs -> flip_flops
  for (int i = 0; i < (int)board_FFs.size(); ++i) {
    Cell* cell = board_FFs[i].first;
    int bit = board_FFs[i].second;
    FF* ff = new FF(cell->getName(), cell->getModel(), cell->getX(), cell->getY(), cell->getW(), cell->getH());
    ff->setBit(bit);
    ff->setClk(1);  // Assuming clk is always 1 for simplicity
    ff->setScan(0); // Assuming scan is always 0 for simplicity
    ff->setRelocateCoor(Coor(-1,-1));
    ff->addPins(cell->getPins());
    flip_flops.push_back(ff);
    FFnameMap[cell->getName()]={ff,i};
  }
  cout<<"finish converting cells to FFs"<<endl;
  // cout << "Total flip flops: " << flip_flops.size() << endl;
  // for (size_t i = 0; i < flip_flops.size(); ++i) {
  //   cout << "FF " << i << ": " << flip_flops[i]->getName() << " at (" 
  //        << flip_flops[i]->getX() << ", " << flip_flops[i]->getY() << ")" << endl;
  // }
  int numberFO=0;
  for(const auto&netName:netlist.getNets()){
    vector<Net> member=netlist.members(netName);
    vector<string> ffIn;
    vector<string> ffOut;
    for(const auto&net:member){
      if(net.getIO()){
        ffIn.push_back(net.getCellName());
      }
      else{
        ffOut.push_back(net.getCellName());
        numberFO++;
      }
    }
    for(const auto&in:ffIn){
      FF* ff=FFnameMap[in].first;
      for(const auto&out:ffOut){
        ff->addNext(FFnameMap[out].second);
      }
    }
  }
  float kp,ka,kt;
  kp=exactPower/(float)numberFO;
  ka=exactArea/(float)flip_flops.size();
  cout<<"finish adding pins"<<endl;
  
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
  kt=exactTNS/total_wire_length;
  Legalization legalize;
  legalize.legalizePlacing(flip_flops,bins,board);
  cout<<"finish legalizing"<<endl;
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



  for(int j=0;j<KmeanIteration;j++){
    
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
      double b = 0.95;
      MBFFgeneration generator(flipflop, maxDrivingStrength, b,alpha,beta,gamma);
      // vector<set<string>> mbff_result = generator.generateMBFF();
      vector<MBFF> placed_mbffs=generator.locationAssignment(bins);
      generator.MBFFsizing(placed_mbffs);
      generator.handleConnection(placed_mbffs);
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