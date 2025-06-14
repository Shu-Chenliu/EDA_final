#include "Pin.h"

Pin::Pin() : 
  name(""),
  layer(""),
  SR(0),
  slack(0){}

Pin::~Pin(){}

// Setter
void Pin::setName(string Name){
  name = Name;
}

void Pin::setLayer(string Layer){
  layer = Layer;
}

void Pin::setCoor(float X, float Y){
  coor.setCoor(X, Y);
}

void Pin::setSR(int sr){
  SR = sr;
}

void Pin::setSlack(float S){
  slack = S;
}

// Getter
string Pin::getName() const{
  return name;
}

string Pin::getLayer() const{
  return layer;
}

float Pin::getX() const{
  return coor.getX();
}

float Pin::getY() const{
  return coor.getY();
}

int Pin::getSR() const{
  return SR;
}

float Pin::getSlack() const{
  return slack;
}