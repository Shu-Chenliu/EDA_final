#include "Pin.h"

Pin::Pin() : 
  name(""),
  SR(0),
  slack(0){}

Pin::~Pin(){}

// Setter
void Pin::setName(string Name){
  name = Name;
}

void Pin::setCoor(double X, double Y){
  coor.setCoor(X, Y);
}

void Pin::setSR(int sr){
  SR = sr;
}

void Pin::setSlack(double S){
  slack = S;
}

// Getter
string Pin::getName() const{
  return name;
}

double Pin::getX() const{
  return coor.getX();
}

double Pin::getY() const{
  return coor.getY();
}

int Pin::getSR() const{
  return SR;
}

double Pin::getSlack() const{
  return slack;
}