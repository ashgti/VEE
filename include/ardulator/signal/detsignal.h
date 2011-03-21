#ifndef ARDULATOR_SIGNAL_DETSIGNAL_H
#define ARDULATOR_SIGNAL_DETSIGNAL_H

#include "ardulator/signal.h"

class DetSignal : public Signal {
  public:
    DetSignal();
    std::string parseConfiguration(std::string);
    double calcNext(int new_state);
    void report(bool);
};

#endif /* ARDULATOR_SIGNAL_DETSIGNAL_H */
