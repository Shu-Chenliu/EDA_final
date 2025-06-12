#ifndef FF_H
#define FF_H

#include "Cell.h"

class FF : public Cell{
  private:
    int bit;
    int clk;
    int scan;

  public:
    FF();
    ~FF();

    // Setter
    void setBit(int n);
    void setClk(int n);
    void setScan(int n);

    // Getter
    int getBit() const;
    int getClk() const;
    int getScan() const;
};

#endif