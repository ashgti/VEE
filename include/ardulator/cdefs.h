// Copyright John Harrison, 2011

#ifndef INCLUDE_ARDULATOR_CDEFS_H_
#define INCLUDE_ARDULATOR_CDEFS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum ValueType {
  VT_DIGITAL = 1 << 0,
  VT_SERIAL = 1 << 1,
  VT_ANALOG = 1 << 2,
  VT_RAND_STRING = 4
};

/**
 * Represents all of the potential values of a signal.
 * Current, this means a C Syle String, a Digital signal (1 or 0) or an analog
 * value, between 0-1024.
**/
union ValueImp {
  char* str;
  uint8_t digital;
  uint16_t analog;
};

/**
**/
struct SignalImp;
struct SignalImp {
  double tick;
  double duration;
  union ValueImp value;
  uint32_t type;
  const char *name;
  void (*cb)(struct SignalImp*);
  struct SignalImp* next;
};

/**
 * Records the history of a signal.
**/
struct History {
  int missed_evts_;   //!< The total number missed events.
  int total_evts_;    //!< The total number of events.
  int caught_evts_;   //!< The total number of events handled.
  double avg_response_time_; //!< The average response time to handle an event.
};

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_ARDULATOR_CDEFS_H_
