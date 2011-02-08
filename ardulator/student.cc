#include "arduino_api.h"
#include <iostream>

void
pinConfiguration() {
    registerPin("a", 0);
    registerPin("b", 1);
    registerPin("c", 2);
    registerPin("d", 3);
    registerPin("e", 4);
    registerPin("f", 5);
}

void
setup() {
    Serial.begin(9600);
    pinMode(0, INPUT);
    pinMode(1, INPUT);
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
}

bool reg[6] = {false};

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
    
    if (digitalRead(1) == HIGH && reg[1] == false) {
        processSignal("b");
        reg[1] = true;
        Serial.println("dispatched b");
    }
    if (digitalRead(1) == LOW && reg[1] == true) {
        reg[1] = false;
    }
    
    if (digitalRead(2) == HIGH && reg[2] == false) {
        processSignal("c");
        reg[2] = true;
        Serial.println("dispatched c");
    }
    if (digitalRead(1) == LOW && reg[2] == true) {
        reg[2] = false;
    }
    
    if (digitalRead(3) == HIGH && reg[3] == false) {
        processSignal("d");
        reg[3] = true;
        Serial.println("dispatched d");
    }
    if (digitalRead(3) == LOW && reg[3] == true) {
        reg[3] = false;
    }
    
    if (digitalRead(4) == HIGH && reg[4] == false) {
        processSignal("e");
        reg[4] = true;
        Serial.println("dispatched e");
    }
    if (digitalRead(4) == LOW && reg[4] == true) {
        reg[4] = false;
    }
    
    if (digitalRead(5) == HIGH && reg[5] == false) {
        processSignal("f");
        reg[5] = true;
        Serial.println("dispatched f");
    }
    if (digitalRead(5) == LOW && reg[5] == true) {
        reg[5] = false;
    }
}