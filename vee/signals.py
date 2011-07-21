__all__ = ['DDet', 'Signal', 'DigitalSignal', 'DDet', 'AnalogSignal']

class Signal(object):
    def __init__(self, pin_id):
        self.pins = [pin_id]
        
    def validate_pin(self):
        assert not hasattr(super(Signal, self), 'validate_pin')

class DigitalSignal(Signal):
    def __init__(self, **kwargs):
        super(DigitalSignal, self).__init__(**kwargs)
    
    def validate_pin(self, pin_id):
        assert pin_id > 0 and pin_id < 14

class SquareWave(Signal):
    def __init__(self, pin_id, high, low, **kwargs):
        super(SquareWave, self).__init__(pin_id, **kwargs)
        self.high = high
        self.low = low

class AnalogSignal(Signal):
    def __init__(self):
        super(AnalogSignal, self).__init__(**kwargs)
        
    def validate_pin(self, pin_id):
        assert 0 > pin_id > 5

class ADet(Signal):
    def __init__(self, **kwargs):
        super(ADet, self).__init__(**kwargs)


class Sin(AnalogSignal):
    def __int__(self, **kwargs):
        super(Sin, self).__init__(**kwargs)

class Composite(object):
    def __init__(self, pins):
        self.pins = pins
