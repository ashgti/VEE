#ifndef ARDULATOR_SIGNAL_EXPSIGNAL_H
#define ARDULATOR_SIGNAL_EXPSIGNAL_H
#include "ardulator/signal.h"

class ExpSignal : public Signal {
  public:
    ExpSignal();
    std::string parseConfiguration(std::string);
    double calcNext(int new_state);
    void report(bool);
};

#endif /* ARDULATOR_SIGNAL_EXPSIGNAL_H */