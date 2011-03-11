#include "arduino_api.h"

bool reg[6] = {false};

const char* signals[6] = { "a", "b", "c", "d", "e", "f" }; 

void
pinConfiguration() {
    for (int i = 0; i < 6; i++) {
        registerPin(signals[i], i);
    }
}

void
setup() {
    Serial.begin(9600);
    for (int i = 0; i < 6; i++) {
        pinMode(i, INPUT);
    }
}

void
loop() {
    for (int i = 0; i < 6; i++) {
        int reading = digitalRead(0);
        if (reading == HIGH && reg[i] == false) {
            processSignal(signals[i]);
            reg[i] = true; // Used to prevent the signal from being dispatched multiple times
            Serial.print("dispatched ");
            Serial.println(signals[i]);
        }
        if (reading == LOW && reg[i] == true) {
            reg[0] = false; // The signal has gone down, so reset the flag to allow it to be dispatched again
        }
    }
}