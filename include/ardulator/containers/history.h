// Copyright John Harrison, 2011

#ifndef INCLUDE_ARDULATOR_CONTAINERS_HISTORY_H_
#define INCLUDE_ARDULATOR_CONTAINERS_HISTORY_H_

namespace ardulator {
namespace containers {

/**
 * Records the history of a signal.
 */
struct History {
  int missed_evts_;   //!< The total number missed events.
  int total_evts_;    //!< The total number of events.
  int caught_evts_;   //!< The total number of events handled.
  double avg_response_time_; //!< The average response time to handle an event.
};

}  // end namesapce containers
}  // end namespace ardulator

#endif  // INCLUDE_ARDULATOR_CONTAINERS_HISTORY_H_
