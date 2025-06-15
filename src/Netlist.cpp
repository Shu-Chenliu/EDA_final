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
  return pinMap.at(net);
}