#ifndef COOR_H
#define COOR_H

#include <iostream>
using namespace std;

class Coor{
  private:
    float x;
    float y;
  
  public:
    Coor();
    Coor(float X, float Y);
    ~Coor();

    void setX(float X);
    void setY(float Y);
    void setCoor(float X, float Y);

    float getX() const;
    float getY() const;

    Coor operator+(const Coor& that) const;
    Coor operator-(const Coor& that) const;

    // Print
    void print();
};

#endif