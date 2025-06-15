#ifndef IOPIN_H
#define IOPIN_H

#include "Pin.h"
#include "Rect.h"

class IOPin : public Pin{
  private:
    string layer;
    Rect size;
  public:
    IOPin();
    ~IOPin();

    // Setter
    void setLayer(string Layer);
    void setSize(float W, float H);
    
    // Getter
    string getLayer() const;
    float getW() const;
    float getH() const;
};

#endif