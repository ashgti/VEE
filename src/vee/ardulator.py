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

# Interface prototypes
run = veelib.run
run.restype = c_double
run.argstypes = [c_double]

#register_signal = veelib.register_signal
#register_signal.restype = c_bool
#register_signal.argstypes = [c_int, c_int, POINTER(c_int)]

REGISTER_CALLBACK_FUNC = CFUNCTYPE(None, c_int, c_int)

class Ardulator(object):
    """
    This represents an instance of the Ardulator Emulator. Currently because
    of the way the emulator calls the functions in the students code you can
    only have 1 instances of the Ardulator class. This could be worked around
    in future versions of the code, but currently its a limitation of the
    emulator.
    """
    def __init__(self):
        """Initalization of the emulator."""
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

