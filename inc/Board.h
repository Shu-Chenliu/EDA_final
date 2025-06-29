#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "IOPin.h"
#include "Cell.h"
#include "Rect.h"
#include "Netlist.h"
using namespace std;

class Board{
  private:
    Rect size;
    Rect binSize;
    Coor binShift;
    int binNumX;
    int binNumY;
    int* dir;  //

    float Alpha;
    float Beta;
    float Gamma;
    float TNS;
    float TPO;
    float Area;

    int unit;
    int cellNum;  //
    int pinNum;  //

    vector<Cell> Cells;
    unordered_map<string, Cell*> CellList;
    vector<pair<Cell*, int>> FFs;
    vector<IOPin> inPins;
    vector<IOPin> outPins;  // merge IOPins ?
    Netlist NetList;

    vector<string> split(const string& s); 
    float norm(const string& s);
    void erase(string& s, char c);
    bool findDir(const string& s);
  
  public:
    Board();
    ~Board();
    Board(Rect size);
    // Setter
    void setSize(float W, float H);
    void setBinW(float W);
    void setBinH(float H);
    void setBinShift(float X, float Y);

    void setAlpha(float A);
    void setBeta(float B);
    void setGamma(float C);
    void setTNS(float s);
    void setTPO(float p);
    void setArea(float a);

    void setInPins(vector<IOPin> &pins);
    void setOutPins(vector<IOPin> &pins);

    // Getter
    Rect getSize() const;
    vector<Cell> getCells()const;
    float getW() const;
    float getH() const;
    float getBinW() const;
    float getBinH() const;
    float getBinShiftX() const;
    float getBinShiftY() const;

    float getAlpha() const;
    float getBeta() const;
    float getGamma() const;
    float getTNS() const;
    float getTPO() const;
    float getArea() const;
    vector<pair<Cell*, int>> getFFs() const;

    Cell* getCell(const string& name);

    const vector<IOPin> &getInPins() const;
    const vector<IOPin> &getOutPins() const;

    Netlist getNetList() const;


    // Read files
    void readWeight(string file);
    void readDef(string file);
    void readSdc(string file);
    void readV(string file);

    // Print
    void print(bool basic = true, bool cells = true, bool pins = true, bool nets = true);

    // Output
    void forMatplotlib(const string& file);
};

#endif