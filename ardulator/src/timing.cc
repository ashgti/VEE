#include "arduino_api.h"
#include "private_config.h"
#include <sstream>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include "time_defs.h"

using namespace std;

void
delay(unsigned long length) {
    stringstream ss; 
    ss << "Delay of " << length << " a "<< MS2T(length) << " vs " << TICKS_PER_SECOND;
    ardu->debug(ss.str());
    ardu->addTicks(MS2T(length)); 
}

