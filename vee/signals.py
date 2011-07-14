__all__ = ['Deterministic']

class Signal(object):
    def __init__(self, pin_id):
        self.pins = [pin_id]
        
    def validate_pin(self):
        assert not hasattr(super(Signal, self), 'draw')

class DigitalSignal(Signal):
    def __init__(self, **kwargs):
        super(DigitalSignal, self).__init__(**kwargs)
    
    def validate_pin(self, pin_id):
        assert pin_id > 0 and pin_id < 14

class DDet(Signal):
    def __init__(self, high, low, **kwargs):
        super(DDet, self).__init__(**kwargs)

class AnalogSignal(Signal):
    def __init__(self):
        super(AnalogSignal, self).__init(**kwargs)
        
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
