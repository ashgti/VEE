#include "arduino_api.h"
#include <iostream>

void
pinConfiguration() {
    registerPin("a", 0);
    registerPin("b", 1);
    registerSerial("c", &Serial);
    registerPin("d", 2);
    registerPin("e", 3);
}

void
setup() {
    Serial.begin(9600);
    pinMode(0, INPUT);
}

bool reg[5] = {false};

void
loop() {
    if (digitalRead(0) == HIGH && reg[0] == false) {
        processSignal("a");
        reg[0] = true;
        Serial.println("dispatched a");
    }
    if (digitalRead(0) == LOW && reg[0] == true) {
        reg[0] = false;
    }
    
    if (digitalRead(1) == HIGH) {
        processSignal("b");
        Serial.println("dispatched b");
    }
    
    if (Serial.available() > 0) {
        int incomingByte = 0;
        incomingByte = Serial.read();
        
        if (incomingByte == 'c') {
            processSignal("c");
        }
    }
    
    delay(100);
    
    if (digitalRead(2) == HIGH) {
        processSignal("d");
    }
    
    if (digitalRead(3) == HIGH) {
        processSignal("e");
    }
}