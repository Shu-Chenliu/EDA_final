#include "Bin.h"

Bin::Bin(int x, int y, const Rect& r):
x_idx(x),
y_idx(y),
area(r){

}

Bin::~Bin(){

}
Rect Bin::getArea()const{
  return area;
}
void Bin::setRank(int r){
  rank=r;
}
bool Bin::getOccupied()const{
  return occupied;
}
int Bin::getRank()const{
  return rank;
}
void Bin::setOccupied(bool o){
  occupied=o;
}
int Bin::getxIndex()const{
  return x_idx;
}
int Bin::getyIndex()const{
  return y_idx;
}