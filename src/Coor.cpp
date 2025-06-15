#include "Coor.h"

Coor::Coor() : 
  x(-1), 
  y(-1){}

Coor::Coor(float X, float Y){
  x = X;
  y = Y;
}

Coor::~Coor(){}

void Coor::setX(float X){
  x = X;
}

void Coor::setY(float Y){
  y = Y;
}

void Coor::setCoor(float X, float Y){
  x = X;
  y = Y;
}

float Coor::getX() const{
  return x;
}

float Coor::getY() const{
  return y;
}

Coor Coor::operator+(const Coor& that) const {
    return Coor(x + that.x, y + that.y);
}

Coor Coor::operator-(const Coor& that) const {
    return Coor(x - that.x, y - that.y);
}

// Print
void Coor::print(){
  
}