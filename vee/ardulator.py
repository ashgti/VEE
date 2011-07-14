"""
Foobar
"""

__author__ = "foo"

__all__ = ['Arduino']
from ctypes import *
from ctypes.util import find_library

global veelib
veelib = cdll.LoadLibrary(find_library('build/libvee'))

# Interface prototypes
run = veelib.run
run.restype = c_double
run.argstypes = [c_double]

register_signal = veelib.register_signal
register_signal.restype = c_bool
register_signal.argstypes = [c_int, c_int, POINTER(c_int)]

REGISTER_CALLBACK_FUNC = CFUNCTYPE(None, c_int, c_int)

class Ardulator(object):
    def __init__(self):
        self.length = 0
        self.current_time = 0
        self.signals = []
        self.failures = 0
        self._pins = []
    
    def run(self, length=None):
        global veelib
        if length == None:
            if self.length - self.current_time < 0:
                return self.current_time
            else:
                self.current_time = run(c_double(self.length - self.current_time))
                return self.current_time
        else:
            self.current_time = run(c_double(length))
            return self.current_time
    
    def add_signal(self, signal):
        self.signals.append[signal]

    def _validate_signals(self, signal):
        assert set(self._pins).isdisjoint(sgianl.pins)
