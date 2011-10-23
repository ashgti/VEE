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

union ValueImp {
  char* str;
  uint8_t digital;
  uint16_t analog;
};

struct SignalImp;
struct SignalImp {
  double tick;
  double duration;
  union ValueImp value;
  uint32_t type;
  const char *name;
  struct SignalImp* next;
};

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_ARDULATOR_CDEFS_H_
