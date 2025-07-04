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
vector<Bin*> generateBins(Board board,unordered_map<int,unordered_map<int,Bin*>>& map) {
	vector<Bin*> bins;
	int x_bins = (board.getW()) / board.getBinW();
	int y_bins = (board.getH()) / board.getBinH();

	for (int i = 0; i < x_bins; ++i) {
		for (int j = 0; j < y_bins; ++j) {
			Rect area(board.getBinW(),board.getBinH(),board.getSize().getX()+i*board.getBinW(),board.getSize().getY()+j * board.getBinH());
      Bin* b=new Bin(i,j,area);
			bins.push_back(b);
      map[i][j]=b;
		}
	}
	return bins;
}
void resetBin(vector<Bin*>& bins){
  for(const auto&bin:bins){
    bin->setOccupied(false);
  }
}

void save_cost_to_file(const std::vector<double>& cost, const std::string& filename = "cost.txt") {
    std::ofstream file(filename);
    for (size_t i = 0; i < cost.size(); ++i) {
        file << i << " " << cost[i] << "\n";
    }
    file.close();
}

void save_results_to_file(const vector<MBFF>& mbffs, const string& filename = "results.txt") {
    ofstream file(filename);
    file << "809.94 610\n5 5 10810 1000 0.074 0.6\n"; // Example header, adjust as needed
    file << mbffs.size() << "\n";
    for (const auto& mbff : mbffs) {
        file << mbff.getX() << " " << mbff.getY() << " "
             << mbff.getW() << " " << mbff.getH() << "\n";
        
    }
    file << "0" << "\n"; // Assuming no additional information, adjust as needed
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
  unordered_map<int,unordered_map<int,Bin*>> binMap;
  vector<Bin*> bins=generateBins(board,binMap);
  Netlist netlist=board.getNetList();
  random_device rd;
  mt19937 gen(rd());
  float exactTNS=board.getTNS();
  float exactPower=board.getTPO();
  float exactArea=board.getArea();
  vector<pair<Cell*, int>> board_FFs = board.getFFs();
  vector<Cell> cells=board.getCells();
  unordered_map<string,pair<Cell*,int>> cellNameMap;
  for(size_t i=0;i<cells.size();i++){
    cellNameMap[cells[i].getName()]={&cells[i],i};
  }
  float alpha=board.getAlpha();
  float beta=board.getBeta();
  float gamma=board.getGamma();
  vector<FF*> flip_flops;
  // FFs -> flip_flops
  int maxDrivingStrength = 1;
  for (size_t i = 0; i < board_FFs.size(); ++i) {
    Cell* cell = board_FFs[i].first;
    int bit = board_FFs[i].second;
    FF* ff = new FF(cell->getName(), cell->getModel(), cell->getX(), cell->getY(), board.getBinW(), board.getBinH());
    ff->setBit(bit);
    ff->setClk(1);  // Assuming clk is always 1 for simplicity
    ff->setScan(0); // Assuming scan is always 0 for simplicity
    ff->addPins(cell->getPins());
    flip_flops.push_back(ff);
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
      if (cellNameMap.find(in) == cellNameMap.end()) {
          cerr << "[ERROR] FFnameMap missing key: " << in << endl;
          continue; // 或 return 或 throw
      }
      Cell* ff=cellNameMap[in].first;
      for(const auto&out:ffOut){
        if (cellNameMap.find(out) == cellNameMap.end()) {
            cerr << "[ERROR] FFnameMap missing key: " << out << endl;
            continue; // 或 return 或 throw
        }
        ff->addNext(cellNameMap[out].second,out);
        cout << "  -> Next FF: " << out << endl;
      }
    }
  }
  cout << "FO count: " << numberFO << endl;
  float kp,ka,kt;
  //TODO: fix estimate method
  kp=exactPower/(float)flip_flops.size();
  ka = exactArea/(board.getBinH()*board.getBinW()*(float)flip_flops.size());
  cout<<kp<<" "<<ka<<endl;
  cout<<"finish adding pins"<<endl;
  Legalization legalize;
  legalize.legalizePlacing(flip_flops,bins,board);
  
  cout<<"finish legalizing"<<endl;
  resetBin(bins);
  cout<<"finish reset bins"<<endl;
  for(const auto&ff:flip_flops){
    cells[cellNameMap[ff->getName()].second].setCoor(ff->getX(),ff->getY());
  }
  // initial wire cost for TNS
  double total_wire_length = 0;
  vector<Edge> edges;
  // turn wire to edge
  cout << "Cell size: " << cells.size() << endl;
  for (size_t i = 0; i < cells.size(); ++i) {
    for (size_t j = 0; j < cells[i].getNext().size(); ++j) {
      edges.push_back(Edge(
        i,
        cells[i].getNext()[j],
        (int)abs(cells[i].getX() - cells[cells[i].getNext()[j]].getX()) +
        abs(cells[i].getY() - cells[cells[i].getNext()[j]].getY())
      )); // Manhattan distance
      
    }
  }
  // for (size_t i = 0; i < cells.size(); ++i) {
  //   for (size_t j = i+1; j < cells.size(); ++j) {
  //     edges.push_back(Edge(
  //       i,
  //       j,
  //       (int)abs(cells[i].getX() - cells[j].getX()) +
  //       abs(cells[i].getY() - cells[j].getY())
  //     )); // Manhattan distance
      
  //   }
  // }
  // do MST
  cout<<edges.size()<<endl;
  MST mst_before(edges, (int)cells.size());
  total_wire_length = mst_before.MinimumSpanningTreeCost();
  cout << "Initial MST wire length: " << total_wire_length << endl;
  kt=exactTNS/total_wire_length;
  cout<<"estimate param"<<kp<<" "<<ka<<" "<<kt<<endl;
  // return 0;
  
  
  int left = 0, right = 0, top = 0, bottom = 0;
  for (auto ff : flip_flops) {
    if (ff->getX() < left) left = ff->getX();
    if (ff->getX() > right) right = ff->getX();
    if (ff->getY() < top) top = ff->getY();
    if (ff->getY() > bottom) bottom = ff->getY();
  }
  size_t size=flip_flops.size();
  float minCost = numeric_limits<float>::max();
  float currentCost = 0;
  float beforeCost = 0;
  int state = 0; // 1: increase, -1: decrease, 0: no change
  bool local_minimum_occur = false;
  int KmeanIteration = 10;//TODO:
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
  cost.push_back(exactArea*gamma+exactPower*beta+exactTNS*alpha);
  MST_costs.push_back(exactTNS);
  Power_cost.push_back(exactPower);
  Area_cost.push_back(exactArea);

  for(int j=0;j<KmeanIteration;j++){
    cout<<"iteration:" <<j<<endl;
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
    unordered_set<string> ffs;
    for(const auto&ff:flip_flops){
      ffs.insert(ff->getName());
      cells[cellNameMap[ff->getName()].second].setRelocateCoor(ff->getRelocateCoor());
    }
    for(auto&c:cells){
      if(ffs.count(c.getName())){
        continue;
      }
      Cluster target;
      float minDis = numeric_limits<float>::max();
      for(const auto&cluster:clusters){
        if(abs(cluster.getX()-c.getX())+abs(cluster.getY()-c.getY())<minDis){
          minDis=abs(cluster.getX()-c.getX())+abs(cluster.getY()-c.getY());
          target=cluster;
        }
      }
      c.setRelocateCoor(Coor(target.getX(),target.getY()));
    }
    //TODO
    edges.clear();
    for (size_t ii = 0; ii < cells.size(); ++ii) {
      for (size_t jj = 0; jj < cells[ii].getNext().size(); ++jj) {
        edges.push_back(Edge(
          ii,
          cells[ii].getNext()[jj],
          (int)abs(cells[ii].getRelocateCoor().getX() - cells[cells[ii].getNext()[jj]].getRelocateCoor().getX()) +
          abs(cells[ii].getRelocateCoor().getY() - cells[cells[ii].getNext()[jj]].getRelocateCoor().getY())
        )); // Manhattan distance   
      }
    }
    // for (size_t ii = 0; ii < flip_flops.size(); ++ii) {
    //   for (size_t jj = ii+1; jj < flip_flops.size(); ++jj) {
    //     edges.push_back(Edge(
    //       ii,
    //       jj,
    //       (int)abs(flip_flops[ii]->getRelocateCoor().getX() - flip_flops[jj]->getRelocateCoor().getX()) +
    //       abs(flip_flops[ii]->getRelocateCoor().getY() - flip_flops[jj]->getRelocateCoor().getY())
    //     )); // Manhattan distance

             
    //   }
    // }
    // do MST
    MST mst_after(edges, (int)cells.size());
    currentMSTCost = mst_after.MinimumSpanningTreeCost();
    // MST_costs.push_back(currentMSTCost);
    // Power_cost.push_back(Power_cost.back());
    // Area_cost.push_back(Area_cost.back());
    // currentCost = board.getAlpha() * MST_costs.back() * kt +
    //               board.getBeta() * (Power_cost.back()) *0.001 +
    //               board.getGamma() * (Area_cost.back());
    cout << "MST wire length after k-means: " << currentMSTCost << endl;
    // vector<string> relocate_records;
    // for (const auto& cell : cells) {
    //   Coor original = Coor(cell.getX(),cell.getY());            // 原始位置
    //   Coor relocated = cell.getRelocateCoor();   // 移動後位置

    //   // 格式: CellName x_orig y_orig x_reloc y_reloc
    //   ostringstream oss;
    //   oss << cell.getName() << " "<<ffs.count(cell.getName())<<" "
    //       << original.getX() << " " << original.getY() << " "
    //       << relocated.getX() << " " << relocated.getY();

    //   relocate_records.push_back(oss.str());
    // }
    // for(int i=0;i<flip_flops.size();i++){
    //   ostringstream oss;
    //   oss << flip_flops[i]->getName() << " "<<i<<" "
    //       << flip_flops[i]->getX() << " " << flip_flops[i]->getY() << " "
    //       << flip_flops[i]->getRelocateCoor().getX() << " " << flip_flops[i]->getRelocateCoor().getY();
    //   relocate_records.push_back(oss.str());
    // }
    // ofstream outfile("relocate_result.txt");
    // if (outfile.is_open()) {
    //     for (const auto& line : relocate_records) {
    //         outfile << line << '\n';
    //     }
    //     outfile.close();
    //     cout << "Relocate results written to relocate_result.txt" << endl;
    // } else {
    //     cerr << "Failed to open relocate_result.txt for writing." << endl;
    // }
    // // break;
    srand(time(0));
    for(int i=0;i< (int)clusters.size();i++){
      vector<FF*> flipflop=clusters[i].flip_flops;
      double b = 0.95;
      MBFFgeneration generator(flipflop, maxDrivingStrength, b,alpha,beta,gamma,kt,kp,ka);
      // vector<set<string>> mbff_result = generator.generateMBFF();
      vector<MBFF> placed_mbffs=generator.locationAssignment(binMap,board,kp);
      generator.MBFFsizing(placed_mbffs);
      current_mbffs.insert(current_mbffs.end(), placed_mbffs.begin(), placed_mbffs.end());
    }
    resetBin(bins);
    unordered_set<string> ff_set;
    for(const auto&ff:flip_flops){
      ff_set.insert(ff->getName());
    }
    
    for(const auto&c:cells){
      if(ff_set.count(c.getName())){
        continue;
      }
      MBFF target;
      float minDis = numeric_limits<float>::max();
      for(const auto&mbff:current_mbffs){
        if(abs(mbff.getCoor().getX()-c.getX())+abs(mbff.getCoor().getY()-c.getY())<minDis){
          minDis=abs(mbff.getCoor().getX()-c.getX())+abs(mbff.getCoor().getY()-c.getY());
          target=mbff;
        }
      }
      target.addMember(c.getName());
    }
    cout<<"finish add member"<<endl;
    unordered_map<string, int> ffToMBFFIndex;
    for (int i = 0; i < (int)current_mbffs.size(); ++i) {
      for (const auto& name : current_mbffs[i].getMembers()) {
        ffToMBFFIndex[name] = i;
      }
    }

    // 接著建立 MBFF 的 next 關係
    for (int i = 0; i < (int)current_mbffs.size(); ++i) {
      unordered_set<int> nextConn;

      for (const auto& memberName : current_mbffs[i].getMembers()) {
        cout<<memberName<<endl;
        Cell* ff = cellNameMap[memberName].first;
        const vector<string>& nexts = ff->getNextName();

        for (const auto& next : nexts) {
          auto it = ffToMBFFIndex.find(next);
          if (it != ffToMBFFIndex.end()) {
            int j = it->second;
            if (j != i) {
              nextConn.insert(j);
            }
          }
        }
      }

      for (int j : nextConn) {
        current_mbffs[i].addNext(j);
      }
    }
    cout<<"finish handle conn"<<endl;
    currentAreaCost = 0;
    currentPowerCost = 0;

    for (const auto& mbff : current_mbffs) {
      currentAreaCost += mbff.getSavedArea();
      currentPowerCost += mbff.getSavedPower();
    }
    // new method estimate power
    // for (int ii = 0; ii < (int)current_mbffs.size(); ++ii) {
    //   currentPowerCost += (double)(current_mbffs[ii].getNext().size()) / (double)sqrt(current_mbffs[ii].getMembers().size());
    // }

    cout<<currentMSTCost<<" "<<exactPower-currentPowerCost<<" "<<exactArea-currentAreaCost<<endl;
    legalize.legalizePlacing(current_mbffs, bins, board);
    cout<<"finish legalizing"<<endl;
    resetBin(bins);
    // MST of MBFF
    // edges.clear();
    // for (int ii = 0; ii < (int)current_mbffs.size(); ++ii) {
    //   for (int jj = jj+1; jj < (int)current_mbffs.size(); ++jj) {
    //     edges.push_back(Edge(
    //       ii,
    //       jj,
    //       (int)abs(current_mbffs[ii].getX() - current_mbffs[jj].getX()) +
    //       abs(current_mbffs[ii].getY() - current_mbffs[jj].getY())
    //     )); // Manhattan distance
    //   }
    // }
    // MST mst_afterMBFF(edges, (int)current_mbffs.size());
    // currentMSTCost = mst_afterMBFF.MinimumSpanningTreeCost();

    MST_costs.push_back(currentMSTCost*kt);
    cout << "MST wire length after k-means: " << currentMSTCost << endl;
    Power_cost.push_back( exactPower-currentPowerCost);
    Area_cost.push_back(exactArea-currentAreaCost);
    
    //TODO: add power and area estimation
    //TODO: update legalization

    // currentCost = board.getAlpha() * MST_costs.back() * kt +
    //               board.getBeta() * (exactPower - Power_cost.back()) +
    //               board.getGamma() * (exactArea-Area_cost.back());
    currentCost = board.getAlpha() * MST_costs.back() +
                  board.getBeta() * Power_cost.back() +
                  board.getGamma() * Area_cost.back();

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
          j = 100;
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
        // local_minimum_occur = false;
      } else if (currentCost > beforeCost) {
        state = 1; // Decrease
      }   
    }
  }
  save_cost_to_file(cost);
  save_all_costs_to_file(MST_costs, Power_cost, Area_cost, cost);
  save_results_to_file(best_mbffs);


  for (auto ff : flip_flops) {
    delete ff;
  }
  for(auto bin:bins){
    delete bin;
  }
  flip_flops.clear();
  bins.clear();
  return 0;
}