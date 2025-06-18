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

  // cout << "Total flip flops: " << flip_flops.size() << endl;
  // for (size_t i = 0; i < flip_flops.size(); ++i) {
  //   cout << "FF " << i << ": " << flip_flops[i]->getName() << " at (" 
  //        << flip_flops[i]->getX() << ", " << flip_flops[i]->getY() << ")" << endl;
  // }
  //TODO: netlist parse
  for(const auto&netName:netlist.getNets()){
    // vector<Net> member=netlist.members(netName);
    // for(const auto&net:member){
    //   string ff1Name=net;
    // }
  }
  Legalization legalize;
  legalize.legalizePlacing(flip_flops,bins,board);
  
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
      vector<MBFF> placed_mbffs=generator.locationAssignment(bins);
      generator.MBFFsizing(placed_mbffs);
      generator.handleConnection(placed_mbffs);
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