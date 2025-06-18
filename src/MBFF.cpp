#include "MBFF.h"

MBFF::MBFF():minSlack(INT_MAX){

}

MBFF::~MBFF(){

}

void MBFF::setMembers(set<string> member){
  for(const auto&m:member){
    members.insert(m);
  }
}
void MBFF::setPosition(Coor coor){
  position=coor;
}
void MBFF::setPins(vector<Pin> p){
  for(int i=0;i<p.size();i++){
    pins.push_back(p[i]);
    if(p[i].getSlack()<minSlack){
      minSlack=p[i].getSlack();
    }
  }
}
void MBFF::setFeasibleRegion(Rect fr){
  feasible_region=fr;
}
void MBFF::setPreferredRegion(Rect pr){
  preferred_region=pr;
}
void MBFF::addNext(int index){
  next.push_back(index);
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