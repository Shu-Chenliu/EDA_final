#ifndef PIN_H
#define PIN_H

#include <iostream>
#include <string>
#include "Coor.h"
using namespace std;

class Pin{
  private:
    string name;
    Coor coor;
    int SR;
    double slack;

  public:
    Pin();
    ~Pin();

    // Setter
    void setName(string Name);
    void setCoor(double X, double Y);
    void setSR(int sr);
    void setSlack(double S);

    // Getter
    string getName() const;
    double getX() const;
    double getY() const;
    int getSR() const;
    double getSlack() const;
};

#endif