// Copyright John Harrison, 2011

#ifndef INCLUDE_AVR_CONFIG_H_
#define INCLUDE_AVR_CONFIG_H_

const uint32_t F_CPU = 16000000;

const uint32_t TICKS_PER_FREQ = (F_CPU / 2);

const uint32_t TICKS_PER_MICRO_SECOND = (1 / 1000000) * TICKS_PER_FREQ;

/** Ticks per cycle **/
const uint32_t TICKS_PER_MILLI_SECOND = (1 / 1000) * TICKS_PER_FREQ;

const uint32_t TICKS_PER_SECOND = TICKS_PER_FREQ;

const uint32_t DIGITAL_READ_TIME = 58;

const uint32_t A2D_PRESCALE = 128;
const uint32_t A2D_CLOCK_COUNT = 13;
const uint32_t A2D_RATE = (F_CPU / A2D_PRESCALE) / A2D_CLOCK_COUNT;

inline uint32_t MS2T(uint32_t x) {
    return x * TICKS_PER_MICRO_SECOND;
}

#endif  // INCLUDE_AVR_CONFIG_H_

