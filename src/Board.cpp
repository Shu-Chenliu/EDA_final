#include "Board.h"

Board::Board() : 
  w(1),
  h(1),
  binW(1),
  binH(1),
  alpha(1),
  beta(1),
  gamma(1),
  TNS(0),
  TPO(0),
  area(1){}

Board::~Board(){}

// Setter
void Board::setW(double W){
  w = W;
}

void Board::setH(double H){
  h = H;
}

void Board::setBinW(double W){
  binW = W;
}

void Board::setBinW(double H){
  binH = H;
}

void Board::setAlpha(double A){
  alpha = A;
}

void Board::setBeta(double B){
  beta = B;
}

void Board::setGamma(double C){
  gamma = C;
}

void Board::setTNS(double tns){
  TNS = tns;
}

void Board::setTPO(double tpo){
  TPO = tpo;
}

void Board::setArea(double A){
  area = A;
}

void Board::setInPins(vector<Pin> &pins){
  inPins = pins;
}

void Board::setOutPins(vector<Pin> &pins){
  outPins = pins;
}

// Getter
double Board::getW() const{
  return w;
}

double Board::getH() const{
  return h;
}

double Board::getBinW() const{
  return binW;
}

double Board::getBinW() const{
  return binH;
}

double Board::getAlpha() const{
  return alpha;
}

double Board::getBeta() const{
  return beta;
}

double Board::getGamma() const{
  return gamma;
}

double Board::getTNS() const{
  return TNS;
}

double Board::getTPO() const{
  return TPO;
}

double Board::getArea() const{
  return area;
}

const vector<Pin> &Board::getInPins(){
  return inPins;
}

const vector<Pin> &Board::getOutPins(){
  return outPins;
}