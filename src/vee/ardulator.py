# Copyright John Harrison, 2011
"""
Ardulator Interfaces

This contains a way to connect to the Emulator Runtime via C interfaces called from ctypes.
"""

__author__ = "John Harrison <ash.gti@gmail.com>"
__all__ = ['Arduino']

import os
import sys
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


## \class PySignalImp
#  Python Implementation of the Signal Data structure.
#  \see SignalImp
class PySignalImp(Structure):
    pass
    # doing this because this is an incomplete type.


SiganlCallback = CFUNCTYPE(None, c_double, POINTER(PySignalImp))

## Python implementation of the ValueImp.
#  \see ValueImp
class PyValueImp(Union):
    _fields_ = [("str", c_char_p),
                ("digital", c_uint8),
                ("analog", c_uint16),
                ]

## Python Implmenetaiton of the History Data Structure.
class PyHistoryImp(Structure):
    _fields_ = [("missed_evts", c_int),
                ("total_evts", c_int),
                ("caught_evts", c_int),
                ("avg_response_time", c_double)]

PySignalImp._fields_ = [("tick", c_double),
                        ("duration", c_double),
                        ("value", PyValueImp),
                        ("type", c_uint32),
                        ("name", c_char_p),
                        ("next", POINTER(PySignalImp))]

# Interface prototypes
## Run command, from C.
run = veelib.run
run.restype = c_double
run.argstypes = [c_double]

## register_signal command, from C.
register_signal = veelib.register_signal
register_signal.restype = c_bool
register_signal.argstypes = [POINTER(PySignalImp)]


## This represents an instance of the Ardulator Emulator. Currently because
#  of the way the emulator calls the functions in the students code you can
#  only have 1 instances of the Ardulator class. This could be worked around
#  in future versions of the code, but currently its a limitation of the
#  emulator.
class PyArdulator(object):
    ## Initalization of the emulator.
    def __init__(self, runtime = 100.0, signals = []):
        ## The estimated runtime length.
        self.length = runtime

        ## The current time, this is useful for when you step through your
        #  program.
        self.current_time = 0

        ## The set of all signals for this scenario
        self.signals = signals

        ## Reference for internal purpose of the data from the config file.
        self._data = {}

        ## A check to see if the signals have ben generated from the config.
        self._signals_generate = False

    ## Do NOT change self.signals after you call run, it will not regenerate
    #  the signals.
    def run_secnario(self, length=None):
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

    ## Generates the C interfacing data from signals.
    def _generate_signal_data(self):
        if self._signals_generate:
            return
        else:
            print 'generating...'
        for s in self.signals:
            rate = self.signals[s].rate
            value = self.signals[s].value

            print 'rate:', rate
            print 'value:', value

            dv = ValueImp()
            dv.digital = c_uint8(1)
            data = SignalImp(0.0, float(rate.duration), dv, value.type_id, s, None)
            orig_data = data
            runtime = 0
            while runtime < self.length:
                next_in = float(rate.next())
                runtime += next_in

                dv = ValueImp()
                dv.digital = c_uint8(value.next_value())
                next_data = SignalImp(runtime, float(rate.duration), dv, value.type_id, s, None)
                data.next = pointer(next_data)
                data = next_data
            self._data[s] = orig_data
        self._signals_generate = True
        for x in self._data:
            register_signal(pointer(self._data[x]))

