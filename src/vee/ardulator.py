# Copyright John Harrison, 2011
"""
Ardulator Interfaces

This contains a way to connect to the Emulator Runtime via C interfaces called from ctypes.
"""

from __future__ import print_function

__author__ = "John Harrison <ash.gti@gmail.com>"
__all__ = ['Arduino']

import os
import sys
import pylab as p
import itertools as i
import ctypes as c
from ctypes.util import find_library

global veelib
# The library itself.
veelib = None
if 'VEE_ROOT' in os.environ:
    veelib = c.cdll.LoadLibrary(find_library(os.environ['VEE_ROOT'] + '/build/libvee'))
else:
    if find_library('build/libvee') == None:
        if os.uname()[0] == 'Linux':
            veelib = c.cdll.LoadLibrary('build/libvee.so')
        else:
            veelib = c.cdll.LoadLibrary('build/libvee.dylib')
    else:
        veelib = c.cdll.LoadLibrary(find_library('build/libvee'))

if not veelib:
    raise ImportError("Could not load the libvee library. Please check your installation.")

# Data Structures

DIGITAL_ID = 1 << 0
SERIAL_ID = 1 << 1
ANALOG_ID = 1 << 2
CALLBACK_ID = 1 << 3
OTHER_ID = 1 << 4

## \class PySignalImp
#  Python Implementation of the Signal Data structure.
#  \see SignalImp
class PySignalImp(c.Structure):
    # doing this because this is an incomplete type.
    def __iter__(self):
        current = self
        while True:
            yield current
            if bool(current.next) == False:
                break
            current = current.next[0]

    def __str__(self):
        value = None
        if self.type == DIGITAL_ID:
            value = self.value.digital
        elif self.type == SERIAL_ID:
            value = self.value.str
        elif self.type == ANALOG_ID:
            value = self.value.analog
        elif self.type == CALLBACK_ID:
            value = '<callback>'
        elif self.type == OTHER_ID:
            value = '<other>'
        else:
            raise ArithmeticError("specified data type not recognized: ")
        return "{{ 'tick': {0!s}, 'duration': {1!s}, 'value': {2!s}, 'type': {3!s}, 'hist': {4!s}}}".format(
                    self.tick,
                    self.duration,
                    value,
                    self.type,
                    str(self.hist))

SignalCallback = c.CFUNCTYPE(None, c.c_double, c.POINTER(PySignalImp))

## Python implementation of the ValueImp.
#  \see ValueImp
class PyValueImp(c.Union):
    _fields_ = [("str", c.c_char_p),
                ("digital", c.c_uint8),
                ("analog", c.c_uint16),
                ('cb', SignalCallback)]

## Python Implmenetaiton of the History Data Structure.
class PyHistoryImp(c.Structure):
    _fields_ = [('caught_at', c.c_double)]

    def __str__(self):
        return '{{ \'caught_at\': {0!s}}}'.format(self.caught_at)


PySignalImp._fields_ = [("tick", c.c_double),
                        ("duration", c.c_double),
                        ("value", PyValueImp),
                        ("type", c.c_uint32),
                        ("name", c.c_char_p),
                        ('hist', PyHistoryImp),
                        ("next", c.POINTER(PySignalImp))]

# Interface prototypes
## Run command, from C.
run = veelib.run
run.restype = c.c_double
run.argstypes = [c.c_double]

## register_signal command, from C.
register_signal = veelib.register_signal
register_signal.restype = c.c_bool
register_signal.argstypes = [c.POINTER(PySignalImp)]

def format_data(name, data, offset=0):
    currently = 0
    changes = []
    for x in data:
        start = x.tick
        if x.hist.caught_at > 0:
            p.axvspan(x.hist.caught_at, x.hist.caught_at + x.duration,
                      alpha=0.25, label='Processing ' + name,
                      color='red')
        if currently != x.value.digital:
            changes.append((start, offset + currently))
            changes.append((start, offset + x.value.digital))
            currently = x.value.digital
    return changes

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
    def run_scenario(self, length=None):
        global veelib
        self._generate_signal_data()
        if length == None:
            if self.length - self.current_time < 0:
                return self.current_time
            else:
                self.current_time = run(c.c_double(self.length
                                                 - self.current_time))
                return self.current_time
        else:
            self.current_time = run(c_double(length))
            return self.current_time

    ## Generates the C interfacing data from signals.
    def _generate_signal_data(self):
        if self._signals_generate:
            return
        else:
            print('generating...')
        for s in self.signals:
            rate = self.signals[s].rate
            value = self.signals[s].value

            print('rate:', rate)
            print('value:', value)

            dv = PyValueImp()
            dv.digital = c.c_uint8(1)
            data = PySignalImp(0.0,
                               float(rate.duration),
                               dv,
                               value.type_id,
                               s,
                               PyHistoryImp(0.0),
                               None)
            orig_data = data
            runtime = 0
            while runtime < self.length:
                next_in = float(rate.next())
                runtime += next_in

                dv = PyValueImp()
                dv.digital = c.c_uint8(value.next_value())
                next_data = PySignalImp(runtime,
                                        float(rate.duration),
                                        dv,
                                        value.type_id,
                                        s,
                                        PyHistoryImp(0.0),
                                        None)
                data.next = c.pointer(next_data)
                data = next_data
            self._data[s] = orig_data
        self._signals_generate = True
        for x in self._data:
            register_signal(c.pointer(self._data[x]))

    ## Generates a report for each pin.
    def generate_reports(self, display_graph=False):
        import itertools as i
        colors = i.cycle(['blue', 'green', 'black'])
        offset = 0
        for signal_name in self.signals:
            pin_history = list(self._data[signal_name])
            caught_signals = dict()
            for i, v in enumerate(pin_history):
                if v.hist.caught_at > 0:
                    caught_signals[i] = v.hist.caught_at
            print('Pin:', signal_name, len(caught_signals), 'of',
                  len(pin_history),)
            print('data: [', ', '.join([str(change) for change in pin_history]), ']', "\n")
            if display_graph:
                changes = format_data(signal_name, pin_history, offset)
                xs, ys = zip(*changes)
                p.plot(xs, ys, color=colors.next())
            offset += 1.5
            p.xlim(-1, max(xs) + 1)
        p.ylim(-0.5, 3.0)
        p.show()

