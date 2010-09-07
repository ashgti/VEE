#include "problem.h"

void setup() {
    pinMode(13, OUTPUT);
    Serial.println("Hi");
}

void loop() {
    Serial.println("looped\n");
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
}