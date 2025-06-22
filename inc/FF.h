#ifndef FF_H
#define FF_H

#include "Cell.h"
class FF : public Cell{
  private:
    int bit;
    int clk;
    int scan;
    
    int cluster;
    

  public:
    FF();
    FF(string name,string modal,float x,float y,float w,float h);
    ~FF();

    // Setter
    void setBit(int n);
    void setClk(int n);
    void setScan(int n);
    void setRelocateCoor(Coor coor)override;
    void setCluster(int cluster);
    void clearNext() override;
    vector<string> getNextName() const override;
    void addNext(int index,string name) override;
    vector<int> getNext() const override;
    // Getter
    int getBit() const;
    int getClk() const;
    int getScan() const;
    Coor getRelocateCoor() const override;
    int getCluster() const;
};

#endif