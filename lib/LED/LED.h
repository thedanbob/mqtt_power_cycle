#ifndef LED_H
#define LED_H

#include <Ticker.h>

class LED {
  public:
    LED();
    void begin();
    void set(bool state);
    void blink();
    void reset();

  private:
    Ticker _timer;
};

#endif
