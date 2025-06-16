#ifndef FF_H
#define FF_H

#include "Cell.h"

class FF : public Cell{
  private:
    int bit;
    int clk;
    int scan;
    Coor relcateCoor;
    int cluster;

  public:
    FF();
    ~FF();

    // Setter
    void setBit(int n);
    void setClk(int n);
    void setScan(int n);
    void setRelocateCoor(Coor coor);
    void setCluster(int cluster);
    // Getter
    int getBit() const;
    int getClk() const;
    int getScan() const;
    Coor getRelocateCoor() const;
    int getCluster() const;
};

#endif