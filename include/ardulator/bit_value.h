// Copyright John Harrison, 2011

#ifndef INCLUDE_ARDULATOR_BIT_VALUE_H_
#define INCLUDE_ARDULATOR_BIT_VALUE_H_

#include <stdint.h>
#include <cstdlib>

namespace ardulator {
namespace containers {

/**
 * A container for a memory register. It contains 8 bits and is used to
 * represent special memory locations of the AVR Atmega328p.
 * Currently, used to decribe the DDRx ports and PORTx ports.
 */
class BitValue {
  bool    ro_;   //!< Configuration of the memory register, is it read-only.
  volatile uint8_t state_; //!< The state of the memory register, contains each
                           //!< of the bits of the memory register.

 public:
  /**
   * Construct an empty BitValue memory register.
   */
  BitValue();
  /**
   * Construct an empty BitValue memory register.
   * @a state Intial state of the memory register.
   * @a readonly Set the readonly state of the register.
   */
  BitValue(uint8_t state, bool readonly = false);
  /**
   * Update the memory register to a given value.
   * @a val the new value for the state of this register.
   */
  void updateState(uint8_t val);
  /**
   * Return the current state of the register.
   */
  uint8_t state() const;

  /**
   * Retrieve a reference to the memory register so you can access it from
   * other places.
   * @see PinConfig::setState
   */
  volatile uint8_t* getStateRef();

  /* Operators */
  /**
   * Assignment overloads, this causes the memory register to act like
   * a signle integer.
   */
  BitValue& operator=(const BitValue &b);
  /**
   * Assignment overloads, this causes the memory register to act like
   * a signle integer.
   */
  BitValue& operator=(const int &b);

  /* Compound-Assignment */
  /**
   * Bitwise XOR and assignment.
   */
  BitValue& operator^=(const BitValue &b);
  /**
   * Bitwise OR and assignment.
   */
  BitValue& operator|=(const BitValue &b);
  /**
   * Bitwise AND and assignment.
   */
  BitValue& operator&=(const BitValue &b);
  /**
   * Bitwise XOR and assignment.
   */
  BitValue& operator^=(const int &b);
  /**
   * Bitwise OR and assignment.
   */
  BitValue& operator|=(const int &b);
  /**
   * Bitwise AND and assignment.
   */
  BitValue& operator&=(const int &b);

  /* Bitwise Operators */
  /**
   * Bitwise NOT.
   */
  BitValue operator~() const;
  /**
   * Bitwise XOR.
   */
  BitValue operator^(const int &b) const;
  /**
   * Bitwise OR.
   */
  BitValue operator|(const int &b) const;
  /**
   * Bitwise AND.
   */
  BitValue operator&(const int &b) const;
  /**
   * Bitwise XOR.
   */
  BitValue operator^(const BitValue &b) const;
  /**
   * Bitwise OR.
   */
  BitValue operator|(const BitValue &b) const;
  /**
   * Bitwise AND.
   */
  BitValue operator&(const BitValue &b) const;

  /**
   * Convert this value into an integer, to make it act like a normal
   * memory register.
   */
  operator int();
};

}  // end namespace containers
}  // end namespace ardulator

#endif  // INCLUDE_ARDULATOR_BIT_VALUE_H_
