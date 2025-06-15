#ifndef NETLIST_H
#define NETLIST_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Pin.h"
using namespace std;

class Netlist{
  private:
    vector<string> Nets;
    unordered_map<string, vector<pair<string, string>>> pinMap;
    // net_name -> ( cell_name/PIN, pin_name )

  public:
    Netlist();
    ~Netlist();

    // Setter
    void addNet(string name, vector<pair<string, string>> pins);

    // Getter
    vector<string> getNets() const;
    vector<pair<string, string>> members(string net) const;
};

#endif