#include "Coor.h"

Coor::Coor() : 
  x(-1), 
  y(-1){}

Coor::Coor(double X, double Y){
  x = X;
  y = Y;
}

Coor::~Coor(){}

void Coor::setX(double X){
  x = X;
}

void Coor::setY(double Y){
  y = Y;
}

void Coor::setCoor(double X, double Y){
  x = X;
  y = Y;
}

double Coor::getX() const{
  return x;
}

double Coor::getY() const{
  return y;
}

Coor Coor::operator+(const Coor& that) const {
    return Coor(x + that.x, y + that.y);
}

Coor Coor::operator-(const Coor& that) const {
    return Coor(x - that.x, y - that.y);
}