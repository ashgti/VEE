#ifndef ARDULATOR_SIGNAL_UNISIGNAL_H
#define ARDULATOR_SIGNAL_UNISIGNAL_H
#include "ardulator/signal.h"

class UniSignal : public Signal {
  public:
    UniSignal();
    std::string parseConfiguration(std::string);
    double calcNext(int new_state);
    void report(bool);
};

#endif /* ARDULATOR_SIGNAL_UNISIGNAL_H */
