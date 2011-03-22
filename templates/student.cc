#include "arduino.h"

void
pinConfiguration() {
    registerPin("a", 1);
}

void
setup() {
    Serial.begin(9600);
    pinMode(1, INPUT);
}

void
loop() {
    processSignal("a");
}