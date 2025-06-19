#include <iostream>
#include <fstream>
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
#include "Bin.h"
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
			bins.push_back(Bin(i,j,area));
		}
	}
	return bins;
}



void save_cost_to_file(const std::vector<double>& cost, const std::string& filename = "cost.txt") {
    std::ofstream file(filename);
    for (size_t i = 0; i < cost.size(); ++i) {
        file << i << " " << cost[i] << "\n";
    }
    file.close();
}

void save_all_costs_to_file(const std::vector<double>& cost1,
                            const std::vector<double>& cost2,
                            const std::vector<double>& cost3,
                            const std::vector<double>& cost4,
                            const std::string& filename = "cost_all.txt") {
    std::ofstream file(filename);
    size_t n = cost1.size(); // assume all vectors are the same size
    for (size_t i = 0; i < n; ++i) {
        file << i << " " << cost1[i] << " " << cost2[i] << " " << cost3[i] << " " << cost4[i] << "\n";
    }
    file.close();
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
  int maxDrivingStrength = 1;
  for (size_t i = 0; i < board_FFs.size(); ++i) {
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
    maxDrivingStrength=max(maxDrivingStrength,bit);
  }
  cout<<"finish converting cells to FFs"<<endl;

  int numberFO=0;
  cout << "getNextSize: " << netlist.getNets().size() << endl;
  for(const auto&netName:netlist.getNets()){
    cout << "Processing net: " << netName << endl;
    vector<Net> member=netlist.members(netName);
    cout << "Net: " << netName << " has " << member.size() << " members." << endl;
    vector<string> ffIn;    
    vector<string> ffOut;
    for(const auto&net:member){
      if(net.getIO()){
        ffIn.push_back(net.getCellName());
        cout << "Input FF: " << net.getCellName() << endl;
      }
      else{
        ffOut.push_back(net.getCellName());
        cout << "Output FF: " << net.getCellName() << endl;
        numberFO++;
      }
    }
    cout << "Net: " << netName << " has " << ffIn.size() << " inputs and " << ffOut.size() << " outputs." << endl;
    // for (int in = 0; in < (int)ffIn.size(); ++in) {
    //   // cout << "in = " << in << endl;
    //   FF* ff=FFnameMap[in].first;
      
    //   for (int out = 0; out < (int)ffOut.size(); ++out) {
    //     ff->addNext(FFnameMap[out].second);
    //     // cout << "out = " << out << endl;
    //   }
    // }
    for(const auto&in:ffIn){
      cout << "Input FF: " << in << endl;
      if (FFnameMap.find(in) == FFnameMap.end()) {
          cerr << "[ERROR] FFnameMap missing key: " << in << endl;
          continue; // 或 return 或 throw
      }
      FF* ff=FFnameMap[in].first;
      for(const auto&out:ffOut){
        if (FFnameMap.find(out) == FFnameMap.end()) {
            cerr << "[ERROR] FFnameMap missing key: " << out << endl;
            continue; // 或 return 或 throw
        }
        ff->addNext(FFnameMap[out].second,out);
        cout << "  -> Next FF: " << out << endl;
      }
    }
  }
  cout << "FO count: " << numberFO << endl;
  float kp,ka,kt;
  //TODO: fix estimate method
  kp=exactPower/(float)numberFO;
  ka=exactArea/(float)flip_flops.size();
  cout<<"estimate param"<<kp<<" "<<ka<<endl;
  cout<<"finish adding pins"<<endl;
  
  
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
  int minCost = numeric_limits<int>::max();
  int currentCost = 0;
  int beforeCost = 0;
  int state = 0; // 1: increase, -1: decrease, 0: no change
  bool local_minimum_occur = false;
  int KmeanIteration = 10;
  double currentMSTCost = 0;
  double currentPowerCost = 0;
  double currentAreaCost = 0;
  vector<double> MST_costs;
  // save results
  vector<MBFF> best_mbffs;
  vector<MBFF> before_mbffs;
  vector<MBFF> current_mbffs;
  vector<double> cost;
  vector<double> Power_cost;
  vector<double> Area_cost;



  for(int j=0;j<KmeanIteration;j++){
    
    beforeCost = currentCost;
    before_mbffs = current_mbffs;
    current_mbffs.clear();
    
    uniform_int_distribution<> num_size((int)flip_flops.size() / 3, (int)flip_flops.size()*2 / 3);
    int SIZE_LIMIT = num_size(gen) ; // Example size limit for clusters
    int MAX_ITER = flip_flops.size() * 2; // Example maximum iterations
    uniform_int_distribution<> num_disp((int)(right - left + bottom - top) / 3, (int)(right - left + bottom - top) *2/ 3);
    int DISP_LIMIT = num_disp(gen);
    kmean kmean(SIZE_LIMIT,MAX_ITER,DISP_LIMIT);
    vector<Cluster> clusters=kmean.kmeansWeighted(flip_flops);
    cout << "K-means clustering completed with " << clusters.size() << " clusters." << endl;
    // edges.clear();
    // for (size_t ii = 0; ii < flip_flops.size(); ++ii) {
    //   for (size_t jj = 0; jj < flip_flops[ii]->getNext().size(); ++jj) {
    //     edges.push_back(Edge(
    //       ii,
    //       flip_flops[ii]->getNext()[jj],
    //       (int)abs(flip_flops[ii]->getRelocateCoor().getX() - flip_flops[flip_flops[ii]->getNext()[jj]]->getRelocateCoor().getX()) +
    //       abs(flip_flops[ii]->getRelocateCoor().getY() - flip_flops[flip_flops[ii]->getNext()[jj]]->getRelocateCoor().getY())
    //     )); // Manhattan distance

             
    //   }
    // }
    // // do MST
    // MST mst_after(edges, (int)flip_flops.size());
    // currentMSTCost = mst_after.MinimumSpanningTreeCost();
    // MST_costs.push_back(currentMSTCost);
    // cout << "MST wire length after k-means: " << currentMSTCost << endl;

    srand(time(0));
    for(size_t i=0;i< (int)clusters.size();i++){
      vector<FF*> flipflop=clusters[i].flip_flops;
      double b = 0.95;
      MBFFgeneration generator(flipflop, maxDrivingStrength, b,alpha,beta,gamma,kt,kp,ka);
      // vector<set<string>> mbff_result = generator.generateMBFF();
      vector<MBFF> placed_mbffs=generator.locationAssignment(bins,board);
      generator.MBFFsizing(placed_mbffs);
      generator.handleConnection(placed_mbffs);
      current_mbffs.insert(current_mbffs.end(), placed_mbffs.begin(), placed_mbffs.end());

    }

    currentAreaCost = 0;
    currentPowerCost = 0;

    for (const auto& mbff : current_mbffs) {
      currentAreaCost += mbff.getSavedArea();
      // currentPowerCost += mbff.getSavedPower();
    }
    // new method estimate power
    for (int ii = 0; ii < (int)current_mbffs.size(); ++ii) {
      currentPowerCost += (double)(current_mbffs[ii].getNext.size()) / (double)(current_mbffs[ii].getMembers().size());
    }


    legalize.legalizePlacing(current_mbffs, bins, board);
    // MST of MBFF
    edges.clear();
    for (int ii = 0; ii < (int)current_mbffs.size(); ++ii) {
      for (int jj = 0; jj < (int)current_mbffs[ii].getNext().size(); ++jj) {
        edges.push_back(Edge(
          ii,
          current_mbffs[ii].getNext()[jj],
          (int)abs(current_mbffs[ii].getX() - current_mbffs[current_mbffs[ii].getNext()[jj]].getX()) +
          abs(current_mbffs[ii].getY() - current_mbffs[current_mbffs[ii].getNext()[jj]].getY())
        )); // Manhattan distance
      }
    }
    MST mst_after(edges, (int)current_mbffs.size());
    currentMSTCost = mst_after.MinimumSpanningTreeCost();
    MST_costs.push_back(currentMSTCost);
    cout << "MST wire length after k-means: " << currentMSTCost << endl;


    
    //TODO: add power and area estimation
    //TODO: update legalization

    currentCost = board.getAlpha() * MST_costs.back() * kt +
                  board.getBeta() * Power_cost.back() * kp +
                  board.getGamma() * Area_cost.back() * ka;

    cost.push_back(currentCost);
    // estimate cost
    if (j == 0) {
      minCost = currentCost;
      beforeCost = currentCost;
      best_mbffs = current_mbffs;
      cout << "============ Initial cost: " << minCost << " =============" << endl;
    } else {
      cout << "============ Current cost: " << currentCost << ", Previous cost: " << beforeCost << " ============="<< endl;
      if (currentCost < beforeCost && state == 1) {
        state = -1; // Decrease
      } else if (currentCost > beforeCost && state == -1) {
        state = 1; // Increase
        if (local_minimum_occur){
          minCost = min(minCost, beforeCost);
          if (beforeCost < minCost) {
            best_mbffs.clear();
            best_mbffs = before_mbffs;
          }
        

          cout << "Iteration " << j << ": Local minimum occurred, stopping optimization." << endl;
          cout << "Cost: " << minCost << endl;
          break; // Stop the loop

        }else{
          local_minimum_occur = true;
          minCost = min(minCost, beforeCost);
          if (beforeCost < minCost) {
            best_mbffs.clear();
            best_mbffs = before_mbffs;
          }
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
  save_cost_to_file(cost);
  save_all_costs_to_file(MST_costs, Power_cost, Area_cost, cost);


  for (auto ff : flip_flops) {
    ff->setRelocateCoor(Coor(ff->getX(),ff->getY()));
  }

  for (auto ff : flip_flops) {
    delete ff;
  }
  flip_flops.clear();
  return 0;
}