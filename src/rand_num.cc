#include <cmath>
#include <cstdlib>
#include <cassert>
#include <ardulator.h>
#include <iostream>
#include <sys/time.h>

//static long int seed_counter = 100;

// RandNum::RandNum(SignalType dis_type, double lambda, unsigned short seed) : _dis_type(dis_type), _lambda(lambda) {
//     assert(lambda > 0);
//     timeval tim;
//     gettimeofday(&tim, NULL);
//     
//     if (seed == 65535) {
//         _Xi[0] = _Xi[1] = _Xi[2] = seed_counter + tim.tv_usec;
//         seed_counter += 72;
//     }
//     else {
//         _Xi[0] = _Xi[1] = _Xi[2] = seed + tim.tv_usec;
//     }
//     _range_start = lambda / 2;
//     _range_end = _range_start + lambda;
// }
//     
// double RandNum::next() {
//     if (_dis_type == ST_UNI) {
//         double val = erand48(_Xi);
//         return _range_start + (_lambda * val);
//     }
//     if (_dis_type == ST_EXP) {
//         double val = erand48(_Xi);
//         return -_lambda * log(val);
//     }
//     
//     return 0;
// }
