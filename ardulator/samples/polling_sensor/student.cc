#include "public_config.h"
#include "arduino_api.h"

void
pinConfiguration() {
    registerPin("a", 0);
    registerPin("b", 1);
    registerPin("c", 13);
    Serial = HardwareSerial(0, 1);
}

void
setup() {
    pinMode(13, OUTPUT);
    Serial.println("Hi");
}

void
loop() {
    if (digitalRead(0) == HIGH)
        processSignal("a");
}