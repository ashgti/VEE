#include <sstream>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include "time_defs.h"
#include "private_config.h"


using namespace std;

void
delay(unsigned long length) {
    ardu->addTicks(MS2T(length)); 
}
 
