#include "Net.h"

Net::Net() : 
  cellName(""),
  pinName(""),
  io(false){}

Net::Net(const string& cell, const string& pin, const bool& dir) : 
  cellName(cell),
  pinName(pin),
  io(dir){}
  
Net::~Net(){}

// Setter
void Net::setCellName(const string& name){
  cellName = name;
}

void Net::setPinName(const string& name){
  pinName = name;
}
void Net::setIO(bool dir){
  io = dir;
}

// Getter
string Net::getCellName() const{
  return cellName;
}

string Net::getPinName() const{
  return pinName;
}

bool Net::getIO() const{
  return io;
}