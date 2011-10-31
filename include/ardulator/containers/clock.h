// Copyright John Harrison, 2011

#ifndef INCLUDE_ARDULATOR_CONTAINERS_CLOCK_H_
#define INCLUDE_ARDULATOR_CONTAINERS_CLOCK_H_

#include <stdint.h>

#include <string>

#include "avr/config.h"

namespace ardulator {
namespace containers {

/**
 * A clock object. Used to keep track of time internal to the emulator.
 * \see TICKS_PER_SECOND
 */
struct Clock {
  /**
   * Takes the number of seconds and the number of ticks.
   * \param seconds the numer of seconds.
   * \param ticks the number of ticks.
  **/
  Clock(uint32_t seconds, uint32_t ticks) :
      seconds_(seconds), ticks_(ticks) { }

  uint32_t seconds_; //!< The seconds on a clock.
  uint32_t ticks_;   //!< The ticks on a clock. @see TICKS_PER_SECOND

  /**
   * A convience function to calculate the current time as a double because
   * in other places we reference time as a floating point number.
  **/
  double now() const {
    return seconds_ + (ticks_ / TICKS_PER_SECOND);
  }

  /**
   * Converts this object to a string.
  **/
  std::string str() const {
    char buffer[1024] = {0};
    snprintf(buffer, 1024, "%d.%05d", seconds_, ticks_);
    return std::string(buffer);
  }
};

}  // end namesapce containers
}  // end namespace ardulator

#endif  // INCLUDE_ARDULATOR_CONTAINERS_CLOCK_H_
