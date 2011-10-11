"""
Ardulator Interfaces

This contains a way to connect to the Emulator Runtime via C interfaces called from ctypes.
"""

__author__ = "John Harrison <ash.gti@gmail.com>"
__all__ = ['Arduino']

import os
from ctypes import *
from ctypes.util import find_library

global veelib
# The library itself.
veelib = None
if 'VEE_ROOT' in os.environ:
    veelib = cdll.LoadLibrary(find_library(os.environ['VEE_ROOT'] + '/build/libvee'))
else:
    veelib = cdll.LoadLibrary(find_library('build/libvee'))

if not veelib:
    raise ImportError("Could not load the libvee library. Please check your installation.")

# Data Structures
class ValueImp(Union):
    _fields_ = [("str", c_char_p),
                ("digital", c_uint8),
                ("analog", c_uint16)]

class SignalImp(Structure):
    pass
    # doing this because this is an incomplete type.

SignalImp._fields_ = [("tick", c_double),
                      ("duration", c_double),
                      ("value", ValueImp),
                      ("type", c_uint32),
                      ("next", POINTER(SignalImp))]

# Interface prototypes

# extern double run(double length)
run = veelib.run
run.restype = c_double
run.argstypes = [c_double]

# extern void initalize_simulator()

# extern bool register_signal(int pin_id, SignalImp* first)
register_signal = veelib.register_signal
register_signal.restype = c_bool
register_signal.argstypes = [c_int, POINTER(SignalImp)]

class Ardulator(object):
    """
    This represents an instance of the Ardulator Emulator. Currently because
    of the way the emulator calls the functions in the students code you can
    only have 1 instances of the Ardulator class. This could be worked around
    in future versions of the code, but currently its a limitation of the
    emulator.
    """
    def __init__(self, runtime = 100.0, signals = []):
        """Initalization of the emulator."""
        self.length = runtime
        self.current_time = 0
        self.signals = signals
        self.failures = 0
        self._data = {}
        self._signals_generate = False

    def run(self, length=None):
        global veelib
        self._generate_signal_data()
        if length == None:
            if self.length - self.current_time < 0:
                return self.current_time
            else:
                self.current_time = run(c_double(self.length - self.current_time))
                return self.current_time
        else:
            self.current_time = run(c_double(length))
            return self.current_time

    def _generate_signal_data(self):
        "Generates the C interfacing data from signals."
        if self._signals_generate:
            return
        else:
            print 'generating...'
        for s in self.signals:
            rate = self.signals[s].rate
            value = self.signals[s].value
            dv = ValueImp()
            dv.digital = c_uint8(1)
            data = SignalImp(0.0, float(rate.duration), dv, value.type_id, None)
            orig_data = data
            runtime = 0
            while runtime < self.length:
                next_in = float(self.signals[s].rate.next())
                runtime += next_in
                
                rate = self.signals[s].rate
                value = self.signals[s].value
                dv = ValueImp()
                dv.digital = c_uint8(1)
                next_data = SignalImp(runtime, float(rate.duration), dv, value.type_id, None)
                data.next = pointer(next_data)
                data = next_data
            self._data[s] = orig_data
        self._signals_generate = True
        for x in self._data:
            register_signal(x, pointer(self._data[x]))
