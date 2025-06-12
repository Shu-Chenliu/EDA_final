#ifndef BOARD_H
#define BOARD_H

#include <iostream>
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
    double getW() const;
    double getH() const;
    double getBinW() const;
    double getBinW() const;

    double setAlpha() const;
    double setBeta() const;
    double setGamma() const;
    double setTNS() const;
    double setTPO() const;
    double setArea() const;
};

#endif