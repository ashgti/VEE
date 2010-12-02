#include "public_config.h"
#include "arduino_api.h"

void
pinConfiguration() {
    setModel(ArduinoMega);
    registerPin("a", 0);
    registerPin("b", 1);
    registerPin("c", 13);
    Serial = HardwareSerial(0, 1);
}

void setup() {
    pinMode(13, OUTPUT);
    Serial.println("Hi");
}

void loop() {
    Serial.println("looped\n");
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1500);
}