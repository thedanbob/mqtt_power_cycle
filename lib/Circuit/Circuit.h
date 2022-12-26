#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <Ticker.h>

class Circuit {
  public:
    Circuit();
    void begin();
    void toggle();

  private:
    Ticker _timer;
};

#endif
