# Copyright John Harrison, 2011

from __future__ import print_function

import random

from ardulator import DIGITAL_ID, SERIAL_ID, ANALOG_ID, CALLBACK_ID, OTHER_ID

__all__ = [ 'Exp', 'Uni', 'Digital', 'Analog', 'Serial',
            'SineWave', 'SquareWave', 'generate_signal']


## Generates a signal from a configuration hash.
def generate_signal(pin_data):
    print(pin_data)
    if pin_data['dataType'] == 'digital':
        value = Digital()
    elif pin_data['dataType'] == 'serial':
        value = None
    elif pin_data['dataType'] == 'analog':
        value = None

    if pin_data['signalType'] == 'exp':
        rate = Exp(pin_data['expLambda'], pin_data['expDuration'])
    elif pin_data['signalType'] == 'uni':
        rate = Uni(pin_data['uniA'], pin_data['uniB'], pin_data['uniDuration'])

    return Signal(rate, value)

## \class Signal
#  Represents a basic signal.
class Signal(object):
    def __init__(self, rate, value):
        self.rate = rate
        self.value = value

## \class Exp
#  Represents an Exponential Distribution on a signal.
class Exp(object):
    def __init__(self, l, duration):
        self.l = float(l)
        self.duration = float(duration)

    def next(self):
        return random.expovariate(1.0 / self.l)

## \class Uni
#  Represents a Uniform Distribution on a signal.
class Uni(object):
    def __init__(self, a, b, duration):
        self.a = float(a)
        self.b = float(b)
        self.duration = float(duration)

    def next(self):
        return random.uniform(self.a, self.b)

## \class Digital
#  Represents a Digital Signal value.
class Digital(object):
    def __init__(self, **kwargs):
        super(Digital, self).__init__(**kwargs)
        self._last_value = 0
        self.type_id = DIGITAL_ID

    def next_value(self):
        self._last_value += 1
        return self._last_value % 2

## \class Serial
#  Represents a serial signal value.
class Serial(object):
    def __init__(self, string, **kwargs):
        self._next_value = string
        self.type_id = SERIAL_ID

    def next_value(self):
        return self._next_value

## \class Analog
#  Represents an analog signal value
class Analog(object):
    def __init__(self, **kwargs):
        super(Analog, self).__init__(**kwargs)
        self.type_id = ANALOG_ID

    def next_value(self, pin_id):
        raise NotImplementedError("Do not use Analog Directly, use a child of\
                                   Analog")

class SineWave(Analog):
    def __init__(self, **kwargs):
        super(SineWave, self).__init__(**kwargs)

    def next_value(self):
        pass

class SquareWave(Analog):
    def __int__(self, **kwargs):
        super(SquareWave, self).__init__(**kwargs)

    def next_value(self):
        pass
