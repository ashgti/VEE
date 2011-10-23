// Copyright John Harrison, 2011

#ifndef INCLUDE_ARDULATOR_PIN_CONFIG_H_
#define INCLUDE_ARDULATOR_PIN_CONFIG_H_

#include <stdint.h>

#include "ardulator/containers.h"

namespace ardulator {

using ::ardulator::containers::History;
using ::ardulator::containers::SignalContainer;
using ::ardulator::containers::Clock;

struct PinConfig {
  /* Constructor */
  explicit PinConfig();

  uint8_t pid_; // Pin Id, eg pin 1, 2, 3...
  bool mode_; // Input/Output
  bool interrupt_;
  bool pull_up_;
  uint8_t state_;
  uint8_t bit_mask_;
  volatile uint8_t *bit_container_;

  History pin_history_;
  SignalContainer signal_;

  void setState(const Clock &t);
  void dispatchingInterrupt(const SignalImp &prev, const SignalImp &next);
};

}  // end namespace ardulator

#endif  // INCLUDE_ARDULATOR_PIN_CONFIG_H_
