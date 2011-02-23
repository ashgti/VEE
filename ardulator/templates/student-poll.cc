#include "arduino_api.h"

void
pinConfiguration() {
    registerPin("a", 0);
}

void
setup() {
    Serial.begin(9600);
    pinMode(0, INPUT);
}

void
loop() {
    if (digitalRead(0) == HIGH) {
        processSignal("a");
        Serial.println("dispatched a");
    }
}