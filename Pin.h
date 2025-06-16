#ifndef PIN_H
#define PIN_H

#include <iostream>
#include <string>
#include "Coor.h"

using namespace std;
class FF;
class Pin{
  private:
    string name;
    string net;
    Coor coor;
    int SR;  // switching rate
    float slack;
    FF* ff;

  public:
    Pin();
    Pin(string Name, string Net,FF* ff);
    ~Pin();

    // Setter
    void setName(string Name);
    void setNet(string Net);
    void setCoor(float X, float Y);
    void setSR(int sr);
    void setSlack(float S);
    void setFF(FF* ff);

    // Getter
    string getName() const;
    string getNet() const;
    float getX() const;
    float getY() const;
    int getSR() const;
    float getSlack() const;
    FF* getFF() const;
    Coor getCoor() const;
    // Print
    void print();
    void printCoor();
};

#endif