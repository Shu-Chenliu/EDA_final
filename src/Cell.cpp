#include "Cell.h"

Cell::Cell : 
  name(""),
  type(""),
  w(0),
  h(0),
  delay(0),
  power(0),
  bit(0),
  pinNum(0){}

Cell::~Cell(){}

// Setter
void Cell::setName(string Name){
  name = Name;
}

void Cell::setType(string Type){
  type = type;
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

void Cell::setBit(int b){
  bit = b;
}

void Cell::setPinNum(int n){ 
  pinNum = n;
} // add one of this which is auto?(count & set)

void addPins(const string &Name, const Coor &Coor){
  pinNames.insert(end(pinNames), begin(Name), end(Name));
  for (int i=0; i<Name.size; i++){
    pinCoorMap.insert(pair<string, Coor>(Name[i], Coor[i]));
  }
}

// Getter
string Cell::getName() const{
  return name;
}

string Cell::getType() const{
  return type;
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

int Cell::getBit() const{
  return bit;
}

int Cell::getPinNum() const{
  return pinNum;
}

const string &Cell::getPinNames() const{
  return pinNames;
}

const Coor &Cell::getPinCoor(const string &pinName) const{
  auto pinCoor = pinCoorMap.find(pinName);
  if (pinCoor == pinCoorMap.end()){
    cout << pinName << " not found in " << name;
  }
  return pinCoor;
}

unordered_map<string, Coor> &Cell::getPinCoorMap() const{
  return pinCoorMap;
}