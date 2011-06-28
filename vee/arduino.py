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
run.restype = c_int

class Arduino(object):
    def __init__(self):
        self.length = 0
        self.pins = []
        self.failures = 0
    
    def run(self):
        global veelib
        return veelib.run()
