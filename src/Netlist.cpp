#include "Netlist.h"

Netlist::Netlist(){}
Netlist::~Netlist(){}

// Setter
void Netlist::addNet(string name, vector<pair<string, string>> pins){
  Nets.push_back(name);
  pinMap.emplace(name, pins);
}

// Getter
vector<string> Netlist::getNets() const{
  return Nets;
}

vector<pair<string, string>> Netlist::members(string net) const{
  auto it = pinMap.find(net);
  if (it != pinMap.end()) return it->second;
  return {};
}

// Print
void Netlist::print(){
  for (auto& net : pinMap){
    cout << net.first << endl;
    for (int i=0; i<(int)net.second.size(); i++){
      cout << "  " << (net.second)[i].first << " " << (net.second)[i].second << endl;
    }
  }
}