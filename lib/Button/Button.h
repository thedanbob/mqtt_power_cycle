#ifndef BUTTON_H
#define BUTTON_H

#include <Ticker.h>
#include "Circuit.h"

class Button {
  public:
    explicit Button(Circuit* c);
    void begin(bool& updateMode);

  private:
    Circuit* _circuitPtr;
    unsigned long _count;
    Ticker _timer;
};

#endif
