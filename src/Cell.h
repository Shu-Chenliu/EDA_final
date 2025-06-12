#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <string>
#include <vector>
#include "Coor.h"
#include "Pin.h"
using namespace std;

class Cell {
  private:
    string name;
    Coor coor;
    double w;
    double h;
    double power;
    double delay;

    vector<Pin> pins;

  public:
    Cell();
    ~Cell();

    // Setter
    void setName(string Name);
    void setX(double X);
    void setY(double Y);
    void setCoor(double X, double Y);

    void setW(double W);
    void setH(double H);
    void setPower(double p);
    void setDelay(double d);

    void setPins(Pin &pins);
    void addPins(Pin &pins);

    // Getter
    string getName() const;
    double getX() const;
    double getY() const;

    double getW() const;
    double getH() const;
    double getPower() const;
    double getDelay() const;

    int getPinNum() const;
    const Pin &getPins() const;
};

#endif