#ifndef RECT_H
#define RECT_H

#include <iostream>
using namespace std;

class Rect{
  private:
    float w;
    float h;

  public:
    Rect();
    Rect(float W, float H);
    ~Rect();

    // Setter
    void setW(float W);
    void setH(float H);
    void setSize(float W, float H);

    // Getter
    float getW() const;
    float getH() const;
};

#endif