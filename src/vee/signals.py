# Copyright (c) 2011 John Harrison

__all__ = [ 'Exp', 'Uni', 'Digital', 'Analog', 'Serial',
            'SineWave', 'SquareWave', 'generate_signal']

def generate_signal(pin_data):
    print pin_data

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

class Signal(object):
    def __init__(self, rate, value):
        self.rate = rate
        self.value = value

    def validate_pin(self):
        assert not hasattr(super(Signal, self), 'validate_pin')

class Exp(object):
    def __init__(self, l, duration):
        self.l = l
        self.duration = duration

    def next(self):
        return self.l

class Uni(object):
    def __init__(self, a, b, duration):
        self.a = a
        self.b = b
        self.duration = duration

# class Data(object):
#     def __init__(self, pin_id):
#         self.pins = [pin_id]

class Digital(object):
    def __init__(self, **kwargs):
        super(Digital, self).__init__(**kwargs)
        self._last_value = 0
        self.type_id = 1 << 0

    def next_value(self):
        self._last_value += 1
        return self._last_value % 2

class Serial(object):
    def __init__(self, string, **kwargs):
        self._next_value = string
        self.type_id = 1 << 1

    def next_value(self):
        return self._next_value

class Analog(object):
    def __init__(self, **kwargs):
        super(Analog, self).__init__(**kwargs)
        self.type_id = 1 << 2

    def next_value(self, pin_id):
        assert 0 > pin_id > 5

class SineWave(object):
    def __init__(self, **kwargs):
        super(SineWave, self).__init__(**kwargs)

class SquareWave(object):
    def __int__(self, **kwargs):
        super(SquareWave, self).__init__(**kwargs)

class Composite(object):
    def __init__(self, pins):
        self.pins = pins
