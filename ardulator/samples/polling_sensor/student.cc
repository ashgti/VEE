// #include "public_config.h"
#include "arduino_api.h"

void
pinConfiguration() {
    registerPin("a", 0);
    registerPin("b", 1);
    registerSerial("c", &Serial);
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
    
    if (digitalRead(1) == HIGH) {
        processSignal("b");
        Serial.println("dispatched b");
    }
    
    if (Serial.available() > 0) {
        int incomingByte = 0;
        incomingByte = Serial.read();
        
        if (incomingByte == 'c')
            processSignal("c");
    }
}