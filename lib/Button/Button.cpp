#include "Button.h"
#include "config.h"
#include "helper.h"

Button::Button(Circuit *c) :
  _count{0},
  _timer{Ticker{}}
{
  _circuitPtr = c;
}

void Button::begin(bool &updateMode) {
  pinMode(BUTTON, INPUT);

  // Put device into update mode (no MQTT) if first button is held during boot
  if (invertedRead(BUTTON)) {
    updateMode = true;
    return;
  }

  // Toggle circuit if button is held for >1 second
  _timer.attach_ms(100, [this]() {
    if (invertedRead(BUTTON)) {
      _count++;
    } else {
      if (_count > 10) _circuitPtr->toggle();
      _count = 0;
    }
  });
}
