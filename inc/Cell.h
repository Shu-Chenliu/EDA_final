#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <string>
#include <vector>
#include <climits>
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
    //TODO: add area and a consturctor
    vector<Pin> pins;
    int minSlack;

  public:
    Cell();
    Cell(string Name, string Model, float X, float Y);
    Cell(string Name, string Model, float X, float Y,float w,float h);
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

    void addPin(Pin pin);
    void addPins(vector<Pin>& Pins);

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
    int getMinSlack() const;
    // Print
    void print();
};

#endif