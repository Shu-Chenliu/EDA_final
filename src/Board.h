#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include "Pin.h"
using namespace std;

class Board{
  private:
    double w;
    double h;
    double binW;
    double binH;

    double alpha;
    double beta;
    double gamma;
    double TNS;
    double TPO;
    double area;

    vector<Pin> inPins;
    vector<Pin> outPins;
  
  public:
    board();
    ~board();

    // Setter
    void setW(double W);
    void setH(double H);
    void setBinW(double W);
    void setBinW(double H);

    void setAlpha(double A);
    void setBeta(double B);
    void setGamma(double C);
    void setTNS(double tns);
    void setTPO(double tpo);
    void setArea(double A);

    void setInPins(vector<Pin> &pins);
    void setOutPins(vector<Pin> &pins);

    // Getter
    double getW() const;
    double getH() const;
    double getBinW() const;
    double getBinW() const;

    double getAlpha() const;
    double getBeta() const;
    double getGamma() const;
    double getTNS() const;
    double getTPO() const;
    double getArea() const;

    const vector<Pin> &getInPins() const;
    const vector<Pin> &getOutPins() const;
};

#endif