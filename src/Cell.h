#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <string>
#include <vector>
#include "Coor.h"
#include "Pin.h"
#include "Rect.h"
using namespace std;

class Cell {
  private:
    string name;
    string model;
    Coor coor;
    Rect size;
    float power;
    float delay;

    vector<Pin> pins;

  public:
    Cell();
    Cell(string Name, string Model, float X, float Y);
    ~Cell();

    // Setter
    void setName(string Name);
    void setModel(string Model);
    void setX(float X);
    void setY(float Y);
    void setCoor(float X, float Y);
    void setSize(float W, float H);
    void setPower(float p);
    void setDelay(float d);

    void setPins(vector<Pin> Pins);
    void addPins(vector<Pin> Pins);

    // Getter
    string getName() const;
    string getModel() const;
    float getX() const;
    float getY() const;
    float getW() const;
    float getH() const;
    float getPower() const;
    float getDelay() const;

    int getPinNum() const;
    const vector<Pin> &getPins() const;
};

#endif