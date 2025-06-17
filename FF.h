#ifndef FF_H
#define FF_H

#include "Cell.h"

class FF : public Cell{
  private:
    int bit;
    int clk;
    int scan;
    Coor relocateCoor;
    int cluster;
    vector<int> next;

  public:
    FF();
    FF(string name,string modal,float x,float y,float w,float h);
    ~FF();

    // Setter
    void setBit(int n);
    void setClk(int n);
    void setScan(int n);
    void setRelocateCoor(Coor coor);
    void setCluster(int cluster);
    void addNext(int index);
    // Getter
    int getBit() const;
    int getClk() const;
    int getScan() const;
    Coor getRelocateCoor() const;
    int getCluster() const;
    vector<int> getNext() const;
};

#endif