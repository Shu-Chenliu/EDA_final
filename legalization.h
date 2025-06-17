#ifndef LEGALIZATION_H
#define LEGALIZATION_N
#include <iostream>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <cmath>
#include "Board.h"
#include "FF.h"
#include "MBFF.h"
using namespace std;
class Legalization{
  public:
    Legalization();
    ~Legalization();
    void legalizePlacing(vector<FF*>& flipflops,vector<Bin>& bins,Board& board);
    void legalizePlacing(vector<MBFF*>& mbffs,vector<Bin>& bins,Board& board);
};

#endif