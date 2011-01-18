#include "public_config.h"
#include "arduino_api.h"

HardwareSerial serial(1, 2);

void
pinConfiguration() {
    registerPin("a", 0);
    registerSerial("b", &serial);
}

void
setup() {
    pinMode(0, INPUT);
}

void
loop() {
    if (digitalRead(0) == HIGH) {
        processSignal("a");
    }
    
    if (serial.available() > 0) {
        int incomingByte = 0;
        incomingByte = serial.read();
        
        if (incomingByte == 'b')
            processSignal("b");
    }
}