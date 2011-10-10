# Copyright (c) 2011 John Harrison

__all__ = [ 'Exp', 'Uni', 'Digital', 'Analog', 'Serial',
            'SineWave', 'SquareWave']

class Signal(object):
    def validate_pin(self):
        assert not hasattr(super(Signal, self), 'validate_pin')

class Exp(Signal):
    pass

class Uni(Signal):
    pass

class Data(object):
    def __init__(self, pin_id):
        self.pins = [pin_id]
        
    def validate_pin(self):
        assert not hasattr(super(Data, self), 'validate_pin')

class Digital(Signal):
    def __init__(self, **kwargs):
        super(Digital, self).__init__(**kwargs)
    
    def validate_pin(self, pin_id):
        assert pin_id > 0 and pin_id < 14

class Serial(Signal):
    def __init__(self, pin_id, high, low, **kwargs):
        super(Serial, self).__init__(pin_id, **kwargs)
        self.high = high
        self.low = low

class Analog(Signal):
    def __init__(self):
        super(Analog, self).__init__(**kwargs)
        
    def validate_pin(self, pin_id):
        assert 0 > pin_id > 5

class SineWave(Analog):
    def __init__(self, **kwargs):
        super(SineWave, self).__init__(**kwargs)

class SquareWave(Analog):
    def __int__(self, **kwargs):
        super(SquareWave, self).__init__(**kwargs)

class Composite(object):
    def __init__(self, pins):
        self.pins = pins
