#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iomanip>
#include <queue>
#include "ardulator.h"

namespace ardulator {

PinConfig::PinConfig() {
}

void PinConfig::setState(ArdulatorClock &t) {
  if (ardu->now() > signal_.current_->tick) {
    signal_.current_ = signal_.current_->next;
  }
}

} // end namespace vee
