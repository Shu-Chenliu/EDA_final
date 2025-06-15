#include "IOPin.h"

IOPin::IOPin(){}

IOPin::~IOPin(){}

// Setter 
void IOPin::setLayer(string Layer){
  layer = Layer;
}

void IOPin::setSize(float W, float H){
  size.setSize(W, H);
}
    
// Getter
string IOPin::getLayer() const{
  return layer;
}

float IOPin::getW() const{
  return size.getW();
}

float IOPin::getH() const{
  return size.getH();
}

// Print 
void IOPin::print(){
  cout << getName() << " " << getNet() << " " << layer << " ";
  size.print();
  cout << "   ";
  printCoor();
}