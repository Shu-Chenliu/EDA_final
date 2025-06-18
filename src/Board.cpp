#include "Board.h"
#include <bits/stdc++.h>
#include <fstream>
#include "Net.h"

Board::Board() : 
  Alpha(1),
  Beta(1),
  Gamma(1),
  TNS(0),
  TPO(0),
  Area(1),
  unit(1000){}

Board::Board(Rect size) : 
  size(size),
  binSize(Rect(1,1,0,0)),
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

void Board::erase(string& s, char c){
  s.erase(remove(s.begin(), s.end(), c), s.end());
}

vector<string> isIn = {"D", "SI", "CK", "A", "A1", "A2"};
vector<string> isOut = {"Q", "QN", "SE", "X"};
bool Board::findDir(const string& s){
  if (find(isIn.begin(), isIn.end(), s) != isIn.end()){
    // Is in
    return true;
  }
  else if (find(isOut.begin(), isOut.end(), s) != isOut.end()){
    // Is out
    return false;
  }
  else{
    if (s.substr(0,2) == "in")  return false;
    else if (s.substr(0,3) == "out")  return true;
    else{
      cout << "% " << s << " not found" << endl;
      return false;
    }
  }
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
Rect Board::getSize() const{
  return size;
}
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

Cell* Board::getCell(const string& name) {
    auto it = CellList.find(name);
    return it != CellList.end() ? it->second : nullptr;
}

// Read files
void Board::readWeight(string file){
  ifstream f(file + "_weight");
  if (!f.is_open()) {
    cerr << "Fail to open weight" << endl;
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
    cerr << "Fail to open def" << endl;
    return;
  }

  string s, ss;
  vector<string> str;
  while (getline(f, s)){
    str = split(s);

    if (str.size() <= 1){ continue; }
    else if (str[0] == "UNITS"){
      unit = stoi(*(str.end()-2));
    }
    else if (str[0] == "DIEAREA"){
      if (str.size() == 10){
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
      else if (getBinH() == 0){
        if (dir == &binNumY)  setBinH(norm(str[4])-getBinShiftY());
        else  setBinW(norm(str[3])-getBinShiftX());
        *dir += 1;
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
      Cells.reserve(cellNum+10);
      for (int i=0; i<cellNum; i++){
        getline(f, s);
        str = split(s);
        // - <comp_name> <model_name> ... + PLACED ( <X> <Y> ) <orientation>
        auto it = find(str.begin(), str.end(), "PLACED");
        Cells.push_back(Cell(str[1], str[2], norm(*(it+2)), norm(*(it+3))));
        CellList.emplace(str[1], &Cells.back());
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
          it = find(str.begin(), str.end(), "NET");
          if (it != str.end()){
            p.setNet(*(it+1));
          }
          it = find(str.begin(), str.end(), "LAYER");
          if (it != str.end()){
            p.setLayer(*(it+1));
            p.setSize(norm(*(it+7))-norm(*(it+3)), norm(*(it+8))-norm(*(it+4)));
          }
          it = find(str.begin(), str.end(), "PLACED");
          if (it != str.end()){
            p.setCoor(norm(*(it+2)), norm(*(it+3)));
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
        vector<Net> pins;
        do{
          getline(f, s);
          str = split(s);
          if (str[0] == "-"){
            netName = str[1];
            erase(netName, '\\');
          }
          auto ptr = find(str.begin(), str.end(), "(");
          while (ptr != str.end()){
            pins.push_back(Net(*(ptr+1), *(ptr+2), findDir(*(ptr+2))));
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
  // I think this could be skipped though?
  ifstream f(file + ".sdc");
  if (!f.is_open()) {
    cerr << "Fail to open sdc" << endl;
    return;
  }

  f.close();
}

void Board::readV(string file){
  ifstream f(file + ".v");
  if (!f.is_open()) {
    cerr << "Fail to open v" << endl;
    return;
  }

  string s, ss;
  vector<string> str;
  while (getline(f, s)){
    str = split(s);
    if (str.size() < 8 ){ continue; }
    else if (str[0] == "module"){
      do{
        getline(f, s);
        str = split(s);
      } while (find(str.begin(), str.end(), ";") == str.end());
    }
    else{
      bool isNew = true, isFF = false;
      int bit = 0;
      Cell *c = getCell(str[1]);
      if(!c)  cout << "empty ptr" << endl;
      do{
        if (isNew)  isNew = false;
        else{
          getline(f, s);
          str = split(s);
        }
        for (int i=0; i<(int)str.size(); i++){
          if (str[i][0] == '.'){
            // 0: pin name, +2: cell name
            erase(str[i+2], '\\');  
            c->addPin(Pin(str[i].substr(1), str[i+2]));
            if (str[i].substr(1) == "CK")  isFF = true;
            else if (str[i][1] == 'D')  bit++;
          }
        }
      } while (find(str.begin(), str.end(), ";") == str.end() );
      if (isFF){
        FFs.push_back(pair<Cell*, int>(c, bit));
      }
      // cout << c->getName() << endl;
      // for (int i=0; i<(int)c->getPins().size(); i++){
      //   cout << " - " << c->getPins()[i].getName() << " " << c->getPins()[i].getNet() << endl;
      // }
    }
  }
  
  f.close();
}

// Print
void Board::print(bool basic, bool cells, bool pins, bool nets){
  if (basic){
    cout << "size: ";
    size.print();
    cout << "binSize: ";
    binSize.print();
    cout << "binShift: ";
    binShift.print();
    cout << "binNum: " << binNumX << " x " << binNumY << endl;

    cout << "\n==================================\n\n";

    cout << "ALpha: " << Alpha << endl;
    cout << "Beta: " << Beta << endl;
    cout << "Gamma: " << Gamma << endl;
    cout << "TNS: " << TNS << endl;
    cout << "TPO: " << TPO << endl;
    cout << "Area: " << Area << endl;
    cout << "unit: " << unit << endl;

    cout << "\n==================================\n\n";
  }

  if (cells){
    cout << "cellNum: " << cellNum << endl;
    for (Cell c : Cells){
      c.print();

      // cout << c.getName() << " ";
      
      // if (c.getName() == "foo1__1"){
      //   c.print();
      //   cout << "YES~" << endl;
      //   getCell(c.getName())->print();
      // }
    }

    cout << "\n==================================\n\n";
  }

  if (pins){
    cout << "pinNum: " << pinNum << endl;
    for (IOPin p : inPins){
      cout << " - ";
      p.print();
    }
    for (IOPin p : outPins){
      cout << " + ";
      p.print();
    }

    cout << "\n==================================\n\n";
  }
  
  if (nets){
    NetList.print();
    cout << "\n==================================\n\n";
  }
}

// Output
void Board::forMatplotlib(const string& file){
  ofstream f (file + ".txt");

  if (f.is_open()){
    f << size.getW() << " " << size.getH() << endl;
    f << binShift.getX() << " " << binShift.getY() << " ";
    f << binNumX << " " << binNumY << " " << binSize.getW() << " " << binSize.getH() << endl;
    
    f << (int)inPins.size() << endl;
    for (auto& ip : inPins){
      f << ip.getX() << " " << ip.getY() << " " << ip.getW() << " " << ip.getH() << endl;
    }

    f << (int)outPins.size() << endl;
    for (auto& op : outPins){
      f << op.getX() << " " << op.getY() << " " << op.getW() << " " << op.getH() << endl;
    }

    f << cellNum << endl;
    for (auto& c : Cells){
      f << c.getX() << " " << c.getY() << endl;
    }

    cout << "Finish write for Matplotlib~~" << endl;
    f.close();
  }

  else cout << "Unable to open file";
}