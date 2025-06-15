#include "Rect.h"

Rect::Rect() : 
  w(0),
  h(0){}

Rect::Rect(float W, float H) : 
  w(W),
  h(H){}

Rect::~Rect(){}

// Setter
void Rect::setW(float W){
  w = W;
}

void Rect::setH(float H){
  h = H;
}

void Rect::setSize(float W, float H){
  w = W;
  h = H;
}

// Getter
float Rect::getW() const{
  return w;
}

float Rect::getH() const{
  return h;
}

// Print
void Rect::print(){
  cout << w << " x " << h << endl;
}