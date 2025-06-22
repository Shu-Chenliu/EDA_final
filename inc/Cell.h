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
    vector<Pin> pins;
    int minSlack;
  protected:
    vector<int> next;
    vector<string> nextName;
    Coor relocateCoor;

  public:
    Cell();
    Cell(string Name, string Model, float X, float Y);
    Cell(string Name, string Model, float X, float Y,float w,float h);
    ~Cell();

    // Setter
    virtual void setRelocateCoor(Coor coor);
    void setName(string Name);
    void setModel(string Model);
    void setX(float X);
    void setY(float Y);
    void setCoor(float X, float Y);
    void setSize(float W, float H);
    void setPower(float p);
    void setDelay(float d);
    virtual void addNext(int index,string name) ;
    void addPin(Pin pin);
    void addPins(const vector<Pin>& Pins);
    virtual void clearNext();
    // Getter
    virtual Coor getRelocateCoor() const;
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
    virtual vector<int> getNext() const;
    virtual vector<string> getNextName() const;
    // Print
    void print();
};

#endif