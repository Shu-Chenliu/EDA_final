#include "FF.h"

FF::FF() : 
  bit(1),
  clk(1),
  scan(-1),
  cluster(-1)
  {}
FF::FF(string name,string modal,float x,float y,float w,float h):
  Cell(name,modal,x,y,w,h){}
FF::~FF(){}

// Setter
void FF::setBit(int n){
  bit = n;
}

void FF::setClk(int n){
  clk = n;
}

void FF::setScan(int n){
  scan = n;
}
void FF::setRelocateCoor(Coor coor){
  coor=coor;
}
void FF::setCluster(int cluster){
  cluster=cluster;
}
// Getter
int FF::getBit() const{
  return bit;
}

int FF::getClk() const{
  return clk;
}

int FF::getScan() const{
  return scan;
}
Coor FF::getRelocateCoor() const{
  return relocateCoor;
}
int FF::getCluster() const{
  return cluster;
}
vector<int> FF::getNext()const{
  return next;
}