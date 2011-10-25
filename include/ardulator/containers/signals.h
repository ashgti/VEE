// Copyright John Harrison, 2011

#ifndef INCLUDE_ARDULATOR_CONTAINERS_SIGNALS_H_
#define INCLUDE_ARDULATOR_CONTAINERS_SIGNALS_H_

#include "ardulator/cdefs.h"

namespace ardulator {
namespace containers {


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

#endif  // INCLUDE_ARDULATOR_CONTAINERS_SIGNALS_H_
