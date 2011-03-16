#include "arduino.h"
#include <iostream>

void blink();

bool reg[4] = {false};

const char* signals[4] = { "a", "b", "c", "d" };

uint32_t int_count = 0;

void
pinConfiguration() {
    for (int i = 0; i < 4; i++) {
        registerPin(signals[i], i);
    }
}

void
setup() {
    Serial.begin(9600);
    for (int i = 0; i < 4; i++) {
        pinMode(i, INPUT);
    }
    
    DDRD = 0b11111111;
    attachInterrupt(1, blink, CHANGE);
}

void
blink() {
    int val = PORTD;
    int_count++;
}

void
loop() {
    if (int_count > 0) {
        std::cout << "Got an interrupt" << std::endl;
        int_count = 0;
    }
    for (int i = 0; i < 4; i++) {
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

