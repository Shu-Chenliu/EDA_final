#ifndef NETLIST_H
#define NETLIST_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Pin.h"
#include "Net.h"
using namespace std;

class Netlist{
  private:
    // vector<string> Nets;
    unordered_map<string, vector<Net>> pinMap;
    // net_name -> ( cell_name/PIN, pin_name )

  public:
    Netlist();
    ~Netlist();

    // Setter
    void addNet(string name, vector<Net> pins);

    // Getter
    // vector<string> getNets() const;
    vector<Net> members(string net) const;

    // Pring
    void print();
};

#endif