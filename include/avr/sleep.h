// Copyright John Harrison, 2011
/**
 * Referecned avr-libc
 */

#ifndef INCLUDE_AVR_SLEEP_H_
#define INCLUDE_AVR_SLEEP_H_

const int SLEEP_MODE_IDLE        = (0)
const int SLEEP_MODE_ADC         = _BV(SM0)
const int SLEEP_MODE_PWR_DOWN    = _BV(SM1)
const int SLEEP_MODE_PWR_SAVE    = (_BV(SM0) | _BV(SM1))
const int SLEEP_MODE_STANDBY     = (_BV(SM1) | _BV(SM2))
const int SLEEP_MODE_EXT_STANDBY = (_BV(SM0) | _BV(SM1) | _BV(SM2))

void set_sleep_mode(int mode);
void sleep_enable();
void sleep_disable();
void sleep_cpu();

#endif  // INCLUDE_AVR_SLEEP_H_

