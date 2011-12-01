// Copyright John Harrison, 2011

#ifndef INCLUDE_ARDULATOR_PIN_CONFIG_H_
#define INCLUDE_ARDULATOR_PIN_CONFIG_H_

#include <stdint.h>

#include "ardulator/cdefs.h"
#include "ardulator/containers/signals.h"
#include "ardulator/containers/clock.h"

namespace ardulator {

using ::ardulator::containers::SignalContainer;
using ::ardulator::containers::Clock;

/**
 * PinConfig is a single pin on the board. It contains all of its
 * configurations and signals related to a single pin.
 * A PinConfig does not have to belong to a pin however. In that case
 * the signal is considered unused and will not dispatch any, instead it
 * counts all the events as missed.
**/
struct PinConfig {
  /* Constructor */
  explicit PinConfig();

  uint8_t pid_;        //!< Pin Id, eg pin 1, 2, 3...
  bool mode_;          //!< Input/Output mode.
  bool interrupt_;     //!< Is this pin an interrupt?
  bool caught_flag_;   //!< Whether or not any signals have been caught.
  bool pull_up_;       //!< Is the pull up resistor enabled?
  uint8_t state_;      //!< The state of the pin, 1 or 0
  uint8_t bit_mask_;   //!< The bit mask for the memory register this pin
                       //!< belongs to.

  volatile uint8_t *bit_container_; //!< A pointer to the location of the
                                    //!< memory address for PORTs.

  SignalContainer signal_; //!< A signal associated with the pin.

  /**
   * Set the pin state, if the time has passed the current pins expiration
   * then update the current signal.
   * @param t The current time.
  **/
  void setState(const Clock &t);
  
  /**
   * Check if dispatch an interrupt is required.
   * @param prev The current Signal.
   * @param next The next Signal.
  **/
  void dispatchingInterrupt(const SignalImp &prev, const SignalImp &next);
};

}  // end namespace ardulator

#endif  // INCLUDE_ARDULATOR_PIN_CONFIG_H_
