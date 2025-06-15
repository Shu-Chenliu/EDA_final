#include "Cell.h"

Cell::Cell() : 
  name(""),
  model(""),
  delay(0),
  power(0){}

Cell::Cell(string Name, string Model, float X, float Y) : 
  name(Name),
  model(Model){
    coor.setCoor(X, Y);
  }

Cell::~Cell(){}

// Setter
void Cell::setName(string Name){
  name = Name;
}

void Cell::setModel(string Model){
  model = Model;
}

void Cell::setX(float X){
  coor.setX(X);
}

void Cell::setY(float Y){
  coor.setY(Y);
}

void Cell::setCoor(float X, float Y){
  coor.setCoor(X, Y);
}

void Cell::setSize(float W, float H){
  size.setSize(W, H);
}

void Cell::setPower(float p){
  power = p;
}

void Cell::setDelay(float d){
  delay = d;
}

void Cell::addPin(Pin pin){
  pins.push_back(pin);
}

void Cell::addPins(vector<Pin> Pins){
  pins.insert(end(pins), begin(Pins), end(Pins));
}

// Getter
string Cell::getName() const{
  return name;
}

string Cell::getModel() const{
  return model;
}

float Cell::getX() const{
  return coor.getX();
}

float Cell::getY() const{
  return coor.getY();
}

float Cell::getW() const{
  return size.getW();
}

float Cell::getH() const{
  return size.getH();
}

float Cell::getPower() const{
  return power;
}

float Cell::getDelay() const{
  return delay;
}

int Cell::getPinNum() const{
  return pins.size();
}

const vector<Pin> &Cell::getPins() const{
  return pins;
}

// Print
void Cell::print(){
  cout << name << " ";
  cout << model << " ";
  cout << power << " ";
  cout /*<< delay*/ << endl;
  cout << "   ";
  coor.print();
  // cout << "   ";
  // size.print();
  for (Pin p : pins){
    cout << "    = ";
    p.print();
  }
}