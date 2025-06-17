#include "Rect.h"

Rect::Rect() : 
  w(0),
  h(0),
  coor(Coor(0,0))
  {}

Rect::Rect(float W, float H,double x,double y) : 
  w(W),
  h(H),
  coor(Coor(x,y))
  {}

Rect::~Rect(){}

// Setter
void Rect::setW(float W){
  w = W;
}

void Rect::setH(float H){
  h = H;
}

void Rect::setSize(float W, float H){
  w = W;
  h = H;
}
void Rect::setX(float x){
  coor.setX(x);
}
void Rect::setY(float y){
  coor.setY(y);
}
void Rect::setName(string n){
  name=n;
}
// Getter
float Rect::getW() const{
  return w;
}

float Rect::getH() const{
  return h;
}
Coor Rect::getCoor() const{
  return coor;
}
float Rect::getX() const{
  return coor.getX();
}
float Rect::getY() const {
  return coor.getY();
}
string Rect::getName() const{
  return name;
}
// Print
void Rect::print(){
  cout << "[ "<< w << " x " << h << " ]" << endl;
}

bool Rect::intersect(const Rect& other){
  return !(coor.getX()+w < other.coor.getX() || other.coor.getX()+w < coor.getX() ||coor.getY()+h < other.coor.getY() || other.coor.getY()+h < coor.getY());
}