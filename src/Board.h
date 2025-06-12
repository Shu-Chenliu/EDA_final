#ifndef BOARD_H
#define BOARD_H

#include <iostream>
using namespace std;

class board{
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

    // Getter
    double getW();
    double getH();
    double getBinW();
    double getBinW();

    double setAlpha(double A);
    double setBeta(double B);
    double setGamma(double C);
    double setTNS(double tns);
    double setTPO(double tpo);
    double setArea(double A);
};

#endif