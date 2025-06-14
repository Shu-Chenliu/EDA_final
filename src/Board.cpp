#include "Board.h"
#include <bits/stdc++.h>

Board::Board() : 
  Alpha(1),
  Beta(1),
  Gamma(1),
  TNS(0),
  TPO(0),
  Area(1),
  unit(1000){}

Board::~Board(){}

// Private
vector<string> Board::split(const string& s){
  istringstream iss(s);
  string ss;
  vector<string> str;
  while (iss >> ss){
    str.push_back(ss);
  }
  return str;
}

float Board::norm(const string& s){
  return stof(s)/unit;
}

// Setter
void Board::setSize(float W, float H){
  size.setSize(W, H);
}

void Board::setBinW(float W){
  binSize.setW(W);
}

void Board::setBinH(float H){
  binSize.setH(H);
}

void Board::setBinShift(float X, float Y){
  binShift.setCoor(X, Y);
}

void Board::setAlpha(float A){
  Alpha = A;
}

void Board::setBeta(float B){
  Beta = B;
}

void Board::setGamma(float C){
  Gamma = C;
}

void Board::setTNS(float s){
  TNS = s;
}

void Board::setTPO(float p){
  TPO = p;
}

void Board::setArea(float a){
  Area = a;
}

void Board::setInPins(vector<IOPin> &pins){
  inPins = pins;
}

void Board::setOutPins(vector<IOPin> &pins){
  outPins = pins;
}

// Getter
float Board::getW() const{
  return size.getW();
}

float Board::getH() const{
  return size.getH();
}

float Board::getBinW() const{
  return binSize.getW();
}

float Board::getBinH() const{
  return binSize.getH();
}

float Board::getBinShiftX() const{
  return binShift.getX();
}

float Board::getBinShiftY() const{
  return binShift.getY();
}

float Board::getAlpha() const{
  return Alpha;
}

float Board::getBeta() const{
  return Beta;
}

float Board::getGamma() const{
  return Gamma;
}

float Board::getTNS() const{
  return TNS;
}

float Board::getTPO() const{
  return TPO;
}

float Board::getArea() const{
  return Area;
}

const vector<IOPin> &Board::getInPins() const{
  return inPins;
}

const vector<IOPin> &Board::getOutPins() const{
  return outPins;
}

// Read files
void Board::readWeight(string file){
  ifstream f(file + "_weight");
  if (!f.is_open()) {
    cerr << "Fail to open weight";
    return;
  }

  string s, type, val;
  while (getline(f, s)){
    stringstream iss(s);
    if (iss >> type >> val){
      if (type == "Alpha")  setAlpha(stof(val));
      else if (type == "Beta")  setBeta(stof(val)); 
      else if (type == "Gamma")  setGamma(stof(val)); 
      else if (type == "TNS")  setTNS(stof(val)); 
      else if (type == "TPO")  setTPO(stof(val)); 
      else if (type == "Area")  setArea(stof(val));
      else  cout << "unknown type " << type << endl; 
    }
  }

  f.close();
}

void Board::readDef(string file){
  ifstream f(file + ".def");
  if (!f.is_open()) {
    cerr << "Fail to open def";
    return;
  }

  string s, ss;
  vector<string> str;
  while (getline(f, s)){
    str = split(s);

    if (str[0] == "UNITS"){
      unit = stoi(str.back());
    }
    else if (str[0] == "DIEAREA"){
      if (str.size() == 5){
        // DIEAREA ( <LLX> <LLY> ) ( <RUX> <RUY> )
        size.setW(norm(str[6])-norm(str[2]));
        size.setH(norm(str[7])-norm(str[3]));
      }
      else{
        // DIEAREA ( <LLX> <LLY> ) ( <LUX> <LUY> ) ( <RUX> <RUY> ) ( <RLX> <RLY> )
        size.setW(norm(str[10])-norm(str[2]));
        size.setH(norm(str[11])-norm(str[3]));
      }
    }
    else if (str[0] == "ROW"){
      //ROW <row_name> <site_name> <origX> <origY> <oriantation> DO <numX> BY <numY> STEP <stepX> <stepY>
      if (binShift.getX() == -1){
        setBinShift(norm(str[3]), norm(str[4]));
        if (stoi(str[9]) == 1){
          dir = &binNumY;
          *dir = 1;
          binNumX = stoi(str[7]);
          setBinW(norm(str[11]));
        } 
        else{
          dir = &binNumX;
          *dir = 1;
          binNumX = stoi(str[9]);
          setBinH(norm(str[12]));
        } 
      }
      else if (getBinH()== -1){
        if (dir == &binNumY)  setBinH(norm(str[4])-getBinShiftY());
        else  setBinW(norm(str[3])-getBinShiftX());
      }
      else{
        *dir += 1;
      }
    }
    else if (str[0] == "TRACKS"){
      continue;
    }
    else if (str[0] == "COMPONENTS"){
      cellNum = stoi(str[1]);
      for (int i=0; i<cellNum; i++){
        getline(f, s);
        str = split(s);
        // - <comp_name> <model_name> ... + PLACED ( <X> <Y> ) <orientation>
        auto it = find(str.begin(), str.end(), "PLACED");
        Cells.push_back(Cell(str[1], str[2], norm(*(it+2)), norm(*(it+3))));
        CellList.emplace(str[1], Cells.end()-1);
      }
      getline(f, s);  // END COMPONENTS
    }
    else if (str[0] == "PINS"){
      pinNum = stoi(str[1]);
      for (int i=0; i<pinNum; i++){
        bool io = 0;
        IOPin p;
        do{
          getline(f, s);
          str = split(s);

          if (str[0] == "-"){
            p.setName(str[1]);
          }
          // auto it = find(str.begin(), str.end(), "NET");
          // if (it != str.end()){}
          auto it = find(str.begin(), str.end(), "DIRECTION");
          if (it != str.end()){
            if (*(it+1) == "INPUT")  io = 1;
          }
          it = find(str.begin(), str.end(), "LAYER");
          if (it != str.end()){
            p.setLayer(*(it+1));
            p.setSize(norm(*(it+4))-norm(*(it+2)), norm(*(it+5))-norm(*(it+3)));
          }
          it = find(str.begin(), str.end(), "PLACED");
          if (it != str.end()){
            p.setCoor(norm(*(it+1)), norm(*(it+2)));
          }
        } while (find(str.begin(), str.end(), ";") == str.end());
        if (io)  inPins.push_back(p);
        else  outPins.push_back(p);
      }
      getline(f, s);  // END PINS
    }
    else if (str[0] == "PINPROPERTIES"){
      do{
        getline(f, s);
      } while (s != "END PINPROPERTIES");
    }
    else if (str[0] == "NETS"){
      int n = stoi(str[1]);
      for (int i=0; i<n; i++){
        string netName;
        vector<pair<string, string>> pins;
        do{
          if (str[0] == "-"){
            netName = str[1];
            netName.erase(remove(netName.begin(), netName.end(), '\\'), netName.end());
          }
          auto ptr = find(str.begin(), str.end(), "(");
          while (ptr != str.end()){
            pins.push_back(pair<string, string>(*(ptr+1), *(ptr+2)));
            ptr = find(ptr+1, str.end(), "(");
          }
        } while (find(str.begin(), str.end(), ";") == str.end());
        NetList.addNet(netName, pins);
      }
    }
  }

  f.close();
}

void Board::readSdc(string file){
  ifstream f(file + ".sdc");
  if (!f.is_open()) {
    cerr << "Fail to open sdc";
    return;
  }

  f.close();
}

void Board::readV(string file){
  ifstream f(file + ".v");
  if (!f.is_open()) {
    cerr << "Fail to open v";
    return;
  }

  f.close();
}