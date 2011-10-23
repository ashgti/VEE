// Copyright John Harrison, 2011

#ifndef INCLUDE_ARDULATOR_CONTAINERS_H_
#define INCLUDE_ARDULATOR_CONTAINERS_H_

#include <stdint.h>

#include "avr/config.h"
#include "ardulator/cdefs.h"

namespace ardulator {
namespace containers {

/**
 * A clock object. Used to keep track of time internal to the emulator.
 * @see TICKS_PER_SECOND
 */
struct Clock {
  /**
   * Takes the number of seconds and the number of ticks.
   * @param seconds the numer of seconds.
   * @param ticks the number of ticks.
   */
  Clock(uint32_t seconds, uint32_t ticks) :
      seconds_(seconds), ticks_(ticks) { }

  uint32_t seconds_; //!< The seconds on a clock.
  uint32_t ticks_;   //!< The ticks on a clock. @see TICKS_PER_SECOND

  /**
   * A convience function to calculate the current time as a double because
   * in other places we reference time as a floating point number.
   */
  double now() const {
    return seconds_ + (ticks_ / TICKS_PER_SECOND);
  }
};

/**
 * Records the history of a signal.
 */
struct History {
  int missed_evts_;   //!< The total number missed events.
  int total_evts_;    //!< The total number of events.
  int caught_evts_;   //!< The total number of events handled.
  double avg_response_time_; //!< The average response time to handle an event.
};

/**
 * A simple container that references the list of SignalImps.
 * Its used to keep track of the head of the list and the current item being
 * used.
 */
struct SignalContainer {
  SignalImp* head_;    //!< First item in the list of SignalImps. In case you
                       //!< need to iterate through the list.
  SignalImp* current_; //!< The current item of the list for dispatching
                       //!< purposes.
};

}  // end namesapce containers
}  // end namespace ardulator

#endif  // INCLUDE_ARDULATOR_CONTAINERS_H_

