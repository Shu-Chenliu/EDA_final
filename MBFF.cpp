#include "MBFF.h"

MBFF::MBFF(){

}

MBFF::~MBFF(){

}

void MBFF::setMembers(set<string>& members){
  members=members;
}
void MBFF::setPosition(Coor coor){
  position=coor;
}
void MBFF::setPins(vector<Pin> pins){
  pins=pins;
}
void MBFF::setFeasibleRegion(Rect feasible_region){
  feasible_region=feasible_region;
}
void MBFF::setPreferredRegion(Rect preferred_region){
  preferred_region=preferred_region;
}
vector<Pin> MBFF::getPins() const{
  return pins;
}
Coor MBFF::getCoor()const{
  return position;
}
float MBFF::getX()const{
  return position.getX();
}
float MBFF::getY()const{
  return position.getY();
}
set<string> MBFF::getMembers()const{
  return members;
}
Rect MBFF::getPreferredRegion()const{
  return preferred_region;
}
Rect MBFF::getFeasibleRegion()const{
  return feasible_region;
}
int MBFF::getDrivingStrength()const{
  return driving_strength;
}
vector<MBFFBit> MBFF::getBits()const{
  return bits;
}