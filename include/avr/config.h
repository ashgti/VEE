
#ifndef AVR_CONFIG_H
#define AVR_CONFIG_H

const long F_CPU = 16000000;
const long TICKS_PER_FREQ = (F_CPU / 2);
const long TICKS_PER_MICRO_SECOND = 1000;
const long TICKS_PER_MILLI_SECOND = 1000000;
const long TICKS_PER_SECOND = TICKS_PER_FREQ;

inline long MS2T(long x) { 
    return x * TICKS_PER_MICRO_SECOND;
}

#endif /* AVR_CONFIG_H */
