/*
  wiring.h - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 804 2009-12-18 16:05:52Z dmellis $
*/

#ifndef INCLUDE_ARDUINO_H_
#define INCLUDE_ARDUINO_H_

/**
 * @file arduino.h
 * @brief The main interface to the emulation system.
 * 
 * To interface to the emulator with roughly the same API you would use in
 * normal Arduino code, include this file and link against the static library
 * provided by VEE.
**/

#include <stdint.h>
#include <stdlib.h>
#include <avr/config.h>

#include <string>

#include "binary.h"
#include "ardulator.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1

#ifndef __cplusplus
#define true 0x1
#define false 0x0
#endif

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define INTERNAL 3
#define DEFAULT 1
#define EXTERNAL 0

#define _BV(bit) (1 << (bit))

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a, b) ((a) < (b)?(a):(b))
#define max(a, b) ((a) > (b)?(a):(b))
#define abs(x) ((x) > 0?(x):-(x))
#define constrain(amt, low, high) ((amt) < (low)?(low):((amt) > (high)? \
                                   (high):(amt)))
#define round(x)     ((x) >= 0 ? (long)((x) + 0.5):(long)((x) - 0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() sei()
#define noInterrupts() cli()

#define clockCyclesPerMicrosecond() (F_CPU / 1000000L)
#define clockCyclesToMicroseconds(a) ((a) / clockCyclesPerMicrosecond())
#define microsecondsToClockCycles(a) ((a) * clockCyclesPerMicrosecond())

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) \
                                                 : bitClear(value, bit))

typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef uint8_t boolean;
typedef uint8_t byte;

void init(void);

/** Set the direction of a pin
 *  \param pin_id The pin id
 *  \param mode The direction of the digital pin.
 */
void pinMode(uint8_t pin_id, uint8_t mode);

/** Write to a pin a given value.
 *  \param pin_id the pin to write to
 *  \param val the value to write, should be HIGH or LOW
 */
void digitalWrite(uint8_t pin_id, uint8_t val);

/** Read a value off a given digital port.
 *  \return Either HIGH or LOW.
 *  \param pin_id The pin you want to check.
 */
int digitalRead(uint8_t pin_id);

int analogRead(uint8_t pin_id);
void analogReference(uint8_t mode);
void analogWrite(uint8_t pin_id, int val);

/** Returns the number of milli seconds since the Arduino started running.
 *  \returns Milliseconds
 */
unsigned long millis(void);

/** Returns the number of micro seconds since the Arduino started running.
 *  \return Microseconds
 */
unsigned long micros(void);

/** Delays the program execution for a given amount of milliseconds.
 *  \param ms Duration of the pause.
 */
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, byte val);

/** Attaches an interrupt to a given pin.
 *  \param pid The pin number of the interrupt.
 *  \param fn The callback function for the interrupt
 *  \param mode The callback conditions.
 */
void attachInterrupt(uint8_t pid, void (*fn)(void), int mode);

/** Removes an interrupt from a given pin.
 *  \param pid The pin number for the interrupt.
 */
void detachInterrupt(uint8_t pid);

#ifdef __cplusplus
}  // extern "C"
#endif

#ifdef __cplusplus
extern "C" {
#endif
/** Enable interrupts **/
void sei();

/** Disable interrupts **/
void cli();

/** Returns from an interrupt routine, enabling global interrupts. This
 * should be the last command executed before leaving an ISR.
 */
void reti();
#ifdef __cplusplus
} /* end extern "C" */
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \fn void setup()
 * \b Important Must be implemented for the Arduino code to work.
 * This is a standard Arduino function. This function is only run once.
 */
extern void   setup() __attribute__((weak));

/**
 * \fn void loop()
 * \b Important Must be implemented for the Arduino code to work.
 * This is a standard Arduino function. This function is run multiple times.
 */
extern void   loop() __attribute__((weak));

/**
 * \fn void pinConfiguration()
 * \b Important Must be implemented in the target Arduino Code.
 * This function maps the signals of the scenario to specific pins for the
 * duration of the scenario.
 * \see registerPin
 * \see registerSerial
 */
extern void   pinConfiguration();

/**
 * Finalizes all of the registered Signal pins.
 */
extern void   report();

/**
 * \fn double run(double length)
 * Run the emulation for a given amount of time and return the time the
 * simulation ran for.
 * \param length How long to run the simulation for.
 */
extern double run(double length);

/**
 * Initialize the simulation.
 */
extern void   initalize_simulator();

/**
 * Reset the state of the simulation.
 */
extern void   reset_simulator();

/**
 * Register a signal used by the simulation.
 * \param head A linked list of signals.
 */
extern bool   register_signal(SignalImp* head);

/**
 * processSignal represents busy work that a processor will do when dispatching
 * a signal.
 * The purpose of this function is to consume CPU cycles since it can be 
 * difficult to represent busy work without a much more targeted approach.
 * \param signal_id The signal you want to process.
 */
void processSignal(const char* signal_id);
/**
 * Map a pin to a signal.
 * \param signal_id The signal you want to map
 * \param pin_id The pin the signal should be connected to
 */
void registerPin(const char* signal_id, uint8_t pin_id);
/**
 * Map a signal to a serial port.
 * This function cheats a bit. It does not check if the hardware actually
 * support Serial communication.
 * \param signal_id The signal you want to map
 * \param serial The serial port you want to connect to
 */
void registerSerial(const char* signal_id, const HardwareSerial &serial);

#ifdef __cplusplus
}  // end extern "C"
#endif

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

/**
 * Printing utilities, requires the implementation of the Print::write function
 * to work.
 */
class Print {
 private:
  void printNumber(unsigned long, uint8_t);
  void printFloat(double, uint8_t);

 public:
  virtual void write(uint8_t) = 0;
  virtual void write(const char *str);
  virtual void write(const uint8_t *buffer, size_t size);

  void print(const char[]);
  void print(const std::string &s);
  void print(char, int = BYTE);
  void print(unsigned char, int = BYTE);
  void print(int, int = DEC);
  void print(unsigned int, int = DEC);
  void print(long, int = DEC);
  void print(unsigned long, int = DEC);
  void print(double, int = 2);

  void println(const char[]);
  void println(const std::string &s);
  void println(char, int = BYTE);
  void println(unsigned char, int = BYTE);
  void println(int, int = DEC);
  void println(unsigned int, int = DEC);
  void println(long, int = DEC);
  void println(unsigned long, int = DEC);
  void println(double, int = 2);
  void println(void);
};

struct ring_buffer;

/** \brief Hardware Serial represents the a basic hardware RS-232 Serial port.
 *  
 *  In practice, the only hardware serial port is contained on a special set of
 *  pins that the hardware supports. This class will work with any set of pins
 *  you pass it, so this is a specific divergence from the actual hardware
 *  implementation.
 */
class HardwareSerial : public Print {
 private:
  uint8_t rxen_; //!< RX Pin
  uint8_t txen_; //!< TX Pin
  uint32_t baud_; //!< Current baud rate

  std::string in_buff_; //!< Internal input buffer. On the Atmel Atmega328p
                        //!< this buffer is limited to 64 characters
  std::string out_buff_; //!< Internal output buffer. On the Atmel Atmega328p
                         //!< this buffer is limited to 64 characters

  FILE* ofile_;  //!< The file the serial port is mapped to

 public:

  /** Creates a serial object, we pretend that any 2 pins will work.
   *  \param rxPin receiver line
   *  \param txPin transmit line
   */
  HardwareSerial(int rxPin, int txPin);

  /** Closes any open files and flushes the buffers **/
  ~HardwareSerial();

  /** Set the baud rate of the serial port. 
   *  Valid rates are: 300, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400,
   *  57600, 115200.
   */
  void begin(long);

  /** Closes the serial port. **/
  void end();

  /** Returns the number of characters in input buffer. **/
  uint8_t available();

  /** Return 1 character from the input buffer and removes that character from
   *  the buffer.
   */
  int read(void);

  /** Flushes the input and output buffers **/
  void flush(void);

  /** Write a single character to the output buffer. **/
  virtual void write(uint8_t);

  /** Returns the rxPin **/
  uint8_t pin() const;

  /** pull in write(str) and write(buf, size) from Print **/
  using Print::write;
};

extern HardwareSerial Serial;

#endif  // INCLUDE_ARDUINO_H_
