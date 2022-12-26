#include "LED.h"
#include "config.h"
#include "helper.h"

#ifndef DISABLE_LINK_LED

LED::LED() :
  _timer{Ticker{}}
{}

void LED::begin() {
  pinMode(LINK_LED, OUTPUT);
  set(HIGH);
}

void LED::set(bool state) {
  invertedWrite(LINK_LED, state);
}

void LED::blink() {
  if (_timer.active()) return;

  _timer.attach_ms(300, []() {
    toggleOutput(LINK_LED);
  });
}

void LED::reset() {
  if (!_timer.active()) return;

  _timer.detach();
  invertedWrite(LINK_LED, HIGH);
}

#else

LED::LED() {}
void LED::begin() {}
void LED::set(bool state) {}
void LED::blink() {}
void LED::reset() {}

#endif
