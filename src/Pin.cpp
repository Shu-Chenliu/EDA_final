#include "Pin.h"

Pin::Pin() : 
  name(""),
  net(""),
  SR(0),
  slack(0){}

Pin::Pin(string Name, string Net) : 
  name(Name),
  net(Net),
  SR(0),
  slack(0){}

Pin::~Pin(){}

// Setter
void Pin::setName(string Name){
  name = Name;
}

void Pin::setNet(string Net){
  net = Net;
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

string Pin::getNet() const{
  return net;
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

// Print 
void Pin::print(){
  cout << name << " " << net << endl;
}