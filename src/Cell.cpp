#include "Cell.h"

Cell::Cell : 
  name(""),
  type(""),
  w(0),
  h(0),
  delay(0),
  power(0){}

Cell::~Cell(){}

// Setter
void Cell::setName(string Name){
  name = Name;
}

void Cell::setX(double X){
  coor.setX(X);
}

void Cell::setY(double Y){
  coor.setY(Y);
}

void Cell::setCoor(double X, double Y){
  coor.setCoor(X, Y);
}

void Cell::setW(double W){
  w = W;
}

void Cell::setH(double H){
  h = H;
}

void Cell::setPower(double p){
  power = p;
}

void Cell::setDelay(double d){
  delay = d;
}

void Cell::setPins(Pin &Pins){
  pins = Pins;
}

void Cell::addPins(Pin &Pins){
  pins.insert(end(pins), begin(Pins), end(Pins));
}

// Getter
string Cell::getName() const{
  return name;
}

double Cell::getX() const{
  return coor.getX();
}

double Cell::getY() const{
  return coor.getY();
}

double Cell::getW() const{
  return w;
}

double Cell::getH() const{
  return h;
}

double Cell::getPower() const{
  return power;
}

double Cell::getDelay() const{
  return delay;
}

int Cell::getPinNum() const{
  return pins.size();
}

const vector<Pin> &Cell::getPins() const{
  return pins;
}