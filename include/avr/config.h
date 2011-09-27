
#ifndef AVR_CONFIG_H
#define AVR_CONFIG_H

const uint32_t F_CPU = 16000000;
const uint32_t TICKS_PER_FREQ = (F_CPU / 2);
const uint32_t TICKS_PER_MICRO_SECOND = 1000;
const uint32_t TICKS_PER_MILLI_SECOND = 1000000;
const uint32_t TICKS_PER_SECOND = TICKS_PER_FREQ;

inline uint32_t MS2T(uint32_t x) { 
    return x * TICKS_PER_MICRO_SECOND;
}

#endif /* AVR_CONFIG_H */
