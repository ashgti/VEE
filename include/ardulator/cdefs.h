// Copyright John Harrison, 2011

#ifndef INCLUDE_ARDULATOR_CDEFS_H_
#define INCLUDE_ARDULATOR_CDEFS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct SignalImp;

/**
 * ValueCallback is a callback for values that require feedback from the
 * runtime to calculate the next status update.
**/
typedef void (*ValueCallback)(double time, struct SignalImp* current_signal);

enum ValueType {
  VT_DIGITAL = 1 << 0,
  VT_SERIAL = 1 << 1,
  VT_ANALOG = 1 << 2,
  VT_OTHER  = 1 << 3,
  VT_RAND_STRING = 1 << 4
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
  ValueCallback cb;
};

struct HistoryImp {
  double caught_at;
};

/**
 * Represents a single signal value.
 */
struct SignalImp {
  double tick; //!< Tick for changinge values
  double duration; //!< Duration the signal goes high
  union ValueImp value; //!< Value of the current tick
  ValueType type; //!< The type of value
  const char *name; //!< The name of the signal
  struct HistoryImp hist; //!< The history of the pin;
  struct SignalImp* next; //!< Pointer to the next signal
};

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_ARDULATOR_CDEFS_H_
