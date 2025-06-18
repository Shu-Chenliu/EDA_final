#ifndef NET_H
#define NET_H

#include <iostream>
#include <string>
using namespace std;

class Net{
  private:
    string cellName;
    string pinName;
    bool io;

  public:
    Net();
    Net(const string& cell, const string& pin, const bool& dir);
    ~Net();

    // Setter
    void setCellName(const string& name);
    void setPinName(const string& name);
    void setIO(bool dir);

    // Getter
    string getCellName() const;
    string getPinName() const;
    bool getIO() const;
};

#endif