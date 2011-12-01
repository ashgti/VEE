# Copyright John Harrison, 2011

## vee-cmd represents the commandline interface to the vee runtime.
#  This program has a few options requires a configuration file to be passed
#  when you start the program.

from __future__ import print_function

from optparse import OptionParser
from pickle import load
import ctypes as c
from vee.signals import *
from vee.ardulator import PyArdulator

parser = OptionParser()
parser.add_option("-f", "--file", dest="filename",
                  help="write output to fileself", metavar="FILE")
parser.add_option("-q", "--quiet",
                  action="store_false", dest="verbose", default=False,
                  help="don't print status messages to stdout")
parser.add_option("-r", "--runtime", dest="runtime", default=100.0,
                  help="runtime length of the scenario")
parser.add_option("-g", "--graph",
                  action="store_false", dest="graph", default=False,
                  help="graph output of signals, requires matplotlib")

def main(options, args):
    """
    Runs the commandline interface to VEE, takes a configuration file as a parameter.
    """
    if len(args) > 0:
        with open(args[0], 'r') as f:
            settings = load(f)
        print(settings)
        signals = {}
        for pin_id in settings:
          signals[pin_id] = generate_signal(settings[pin_id])
        # signals = {pin_id: generate_signal(settings[pin_id]) for pin_id in settings}
        print("signals:", signals)
        a = PyArdulator(options.runtime, signals)
        # a.length = 50.0 # Another way of setting the secnario length
        # a.signals = signals # Updating the signals
        print(a.run_scenario())

        a.generate_reports(True) # options.graph)

        print('Total runtime:', a.length)
    else:
        print("Please pass the program a configuration file.")

if __name__ == '__main__':
    (options, args) = parser.parse_args()
    main(options, args)

