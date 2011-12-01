// Copyright John Harrison, 2011

#include <iostream>
#include <string>

#include "arduino.h"

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
  // std::cout << "bv: " << _BV(1) << std::endl;
  int val = digitalRead(0);
  // int val = (PORTD & _BV(1)) ? 1 : 0;
  int_count++;
  PORTB |= _BV(5);
}

void loop() {
  if (int_count > 0) {
    int_count = 0;
  }
  if (PORTB & _BV(5)) {
    PORTB = 0;
  }
  for (int i = 0; i < 4; i++) {
    int reading = digitalRead(i);
    if (reading == HIGH && reg[i] == false) {
      processSignal(signals[i]);
      reg[i] = true; 
      Serial.print("dispatched ");
      Serial.println(signals[i]);
    }
    if (reading == LOW && reg[i] == true) {
      reg[i] = false;
    }
  }
  // std::cout << "End loop..." << std::endl;
}

