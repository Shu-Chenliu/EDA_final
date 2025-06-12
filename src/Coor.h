#ifndef COOR_H
#define COOR_H

#include <iostream>
using namespace std;

class Coor {
  private:
    double x;
    double y;
  
  public:
    Coor();
    Coor(double x, double y);
    ~Coor();

    void setX(double x);
    void setY(double y);
    void setCoor(double x, double y);

    double getX() const;
    double getY() const;

    Coor Operator+(const Coor that) const;
    Coor Operator-(const Coor that) const;
    
};

#endif