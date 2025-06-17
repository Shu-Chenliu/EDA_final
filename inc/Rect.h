#ifndef RECT_H
#define RECT_H

#include <iostream>
#include "Coor.h"
using namespace std;

class Rect{
  private:
    float w;
    float h;
    Coor coor;
    string name;

  public:
    Rect();
    Rect(float W, float H, double x, double y);
    ~Rect();

    // Setter
    void setW(float W);
    void setH(float H);
    void setSize(float W, float H);
    void setX(float x);
    void setY(float y);
    void setName(string name);

    // Getter
    float getW() const;
    float getH() const;

    Coor getCoor() const;
    float getX() const;
    float getY() const;
    string getName() const;
    // Print
    void print();
    bool intersect(const Rect& other);
};

#endif