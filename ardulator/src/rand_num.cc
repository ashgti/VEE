#include <cmath>
#include <cstdlib>
#include <private_config.h>

template <class T> 
RandNum<T>::RandNum(Distribution dis_type, T lambda, unsigned short seed) : _dis_type(dis_type), _lambda(lambda) {
    assert(lambda > 0);
    _Xi[0] = _Xi[1] = 0; _Xi[2] = seed;
    _range_start = lambda / 2;
    _range_end = _range_start + lambda;
}
    
template <class T>
T RandNum<T>::next() {
    if (_dis_type == UNI) {
        double val = erand48(_Xi);
        return _range_start + (_lambda * val);
    }
    if (_dis_type == EXP) {
        double val = erand48(_Xi);
        return _range_start + _lambda * sin(M_PI * val);
    }
}
