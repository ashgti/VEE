// Copyright John Harrison, 2011

#include <cmath>
#include <cassert>
#include <queue>

#include "ardulator/pin_config.h"
#include "arduino.h"

namespace ardulator {

using ::ardulator::containers::Clock;

PinConfig::PinConfig() : pid_(0) {
}

void PinConfig::setState(const Clock &t) {
  if (t.now() > signal_.current_->tick) {
    if (signal_.current_->type == VT_DIGITAL) {
      if (bit_container_) {
        if (signal_.current_->next->value.digital == HIGH) {
          *bit_container_ |= bit_mask_ & 0xff;
        } else {
          *bit_container_ |= bit_mask_ & 0x00;
        }
      }
    }

    if (interrupt_)
      dispatchingInterrupt(*signal_.current_, *signal_.current_->next);

    signal_.current_ = signal_.current_->next;
  }
}

// TODO(ashgti): Fixup this function. Needs to dispatch interrupts.
void PinConfig::dispatchingInterrupt(const SignalImp &prev, const SignalImp &next) {
  fprintf(ardu->debug_, "Signal: %d switched.", pid_);
  // if (new_state == HIGH) {
  //   fprintf(ardu->debug_, "HIGH");
  // } else {
  //   fprintf(ardu->debug_, "LOW");
  // }
  // print_clock(ardu->debug_, " @ ",
  // _state = new_state;
  Ardulator::InterruptIterator int_iter;

  if (ardu->interrupts_enabled_ == true &&
     (int_iter = ardu->interrupts_.find(pid_)) != ardu->interrupts_.end()) {
    if (next.value.digital == LOW) {
      // LOW EVENT
      if (int_iter->second.first == LOW) {
        int_iter->second.second();
      }
    }
    if (prev.value.digital == LOW && next.value.digital == HIGH) {
      // FIRE RISING EVENT
      if (int_iter->second.first == RISING) {
        int_iter->second.second();
      }
      // _history.total_evts += 1;
      // _caught_flag = false;
    }
    if (prev.value.digital == HIGH && next.value.digital == LOW) {
      // FIRE FALLING EVENT
      if (int_iter->second.first == FALLING) {
        int_iter->second.second();
      }
      // if (_caught_flag == false) {
          // _history.missed_evts += 1;
      // }
    }
    if (prev.value.digital != next.value.digital) {
      // FIRE CHANGED EVENT
      if (int_iter->second.first == CHANGE) {
        int_iter->second.second();
      }
    }
  }
}

}  // end namespace vee

