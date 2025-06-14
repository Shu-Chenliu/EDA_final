#ifndef IOPIN_H
#define IOPIN_H

#include "Pin.h"
#include "Rect.h"

class IOPin : public Pin{
  private:
    Rect size;
  public:
    IOPin();
    ~IOPin();

    // Setter
    void setSize(float W, float H);
    
    // Getter
    float getW() const;
    float getH() const;
};

#endif