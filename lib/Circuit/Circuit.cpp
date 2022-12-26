#include "Circuit.h"
#include "config.h"

Circuit::Circuit() :
  _timer{Ticker{}}
{}

void Circuit::begin() {
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, DEFAULT_STATE);
}

void Circuit::toggle() {
  digitalWrite(RELAY, !DEFAULT_STATE);
  _timer.once(TOGGLE_DURATION, []() {
    digitalWrite(RELAY, DEFAULT_STATE);
  });
}
