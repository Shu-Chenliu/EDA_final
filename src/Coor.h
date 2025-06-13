#ifndef COOR_H
#define COOR_H

#include <iostream>
using namespace std;

class Coor{
  private:
    double x;
    double y;
  
  public:
    Coor();
    Coor(double X, double Y);
    ~Coor();

    void setX(double X);
    void setY(double Y);
    void setCoor(double X, double Y);

    double getX() const;
    double getY() const;

    Coor operator+(const Coor& that) const;
    Coor operator-(const Coor& that) const;
};

#endif