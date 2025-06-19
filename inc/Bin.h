#ifndef BIN_H
#define BIN_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "IOPin.h"
#include "Cell.h"
#include "Rect.h"
#include "Netlist.h"
using namespace std;

class Bin{
  private:
    int x_idx, y_idx;
    Rect area;
    int rank;
    bool occupied;
  
  public:
    Bin(int x, int y, const Rect& r);
    ~Bin();
    Rect getArea()const;
    void setRank(int r);
    bool getOccupied()const;
    int getRank()const;
    void setOccupied(bool o);
    int getxIndex()const;
    int getyIndex()const;
};

#endif