#include "arduino.h"
#include <iostream>
#include <string>

using namespace std;

void blink();

bool reg[4] = {false};

const char* signals[4] = { "a", "b", "c", "d" };

volatile uint32_t int_count = 0;

HardwareSerial Serial2(20,21);

void pinConfiguration() {
  for (int i = 0; i < 4; i++) {
    registerPin(signals[i], i);
  }

  registerSerial("e", Serial2);
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(i, INPUT);
  }

  attachInterrupt(0, blink, CHANGE);
}

void blink() {
  int val = PORTD & _BV(1);
  std::cout << "val: " << val << std::endl;
  int_count++;
  PORTB |= _BV(5);
}

void loop() {
  if (int_count > 0) {
    std::cout << "Student File: Got an interrupt" << std::endl;
    int_count = 0;
  }
  if (PORTB & _BV(5)) {
    std::cout << "Student File: Yes it worked " << std::endl;
    PORTB = 0;
  }
  for (int i = 0; i < 4; i++) {
    int reading = digitalRead(0);
    if (reading == HIGH && reg[i] == false) {
      if (string(signals[i]) == "b") {
          cout << "Stduent File: Yes..." << endl;
      }
      // processSignal(signals[i]);
      reg[i] = true; 
      Serial.print("dispatched ");
      Serial.println(signals[i]);
    }
    if (reading == LOW && reg[i] == true) {
      reg[0] = false; 
    }
  }
  // std::cout << "End loop..." << std::endl;
}

