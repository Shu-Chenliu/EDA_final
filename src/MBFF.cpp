#include "MBFF.h"

MBFF::MBFF():
  minSlack(INT_MAX),
  savedPower(0),
  savedArea(0){

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
void MBFF::setDrivingStrength(int ds){
  driving_strength=ds;
}
void MBFF::setX(float x){
  position.setX(x);
}
void MBFF::setY(float y){
  position.setY(y);
}
void MBFF::setH(float h){
  H=h;
}
void MBFF::setW(float w){
  W=w;
}
void MBFF::addSavedPower(double power){
  savedPower+=power;
}
void MBFF::addSavedArea(double area){
  savedArea+=area;
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
int MBFF::getMinSlack()const{
  return minSlack;
}
float MBFF::getW()const{
  return W;
}
float MBFF::getH()const{
  return H;
}