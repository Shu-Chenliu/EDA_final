#include "IOPin.h"

IOPin::IOPin(){}

IOPin::~IOPin(){}

// Setter 
void IOPin::setSize(float W, float H){
  size.setSize(W, H);
}
    
// Getter
float IOPin::getW() const{
  return size.getW();
}

float IOPin::getH() const{
  return size.getH();
}