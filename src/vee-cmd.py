# Copyright John Harrison, 2011

from optparse import OptionParser
from pickle import load
from vee.signals import *
from vee.ardulator import Ardulator

parser = OptionParser()
parser.add_option("-f", "--file", dest="filename",
                  help="write output to fileself", metavar="FILE")
parser.add_option("-q", "--quiet",
                  action="store_false", dest="verbose", default=True,
                  help="don't print status messages to stdout")
parser.add_option("-r", "--runtime", dest="runtime", default=100.0,
                  help="runtime length of the scenario")


def main(options, args):
    if len(args) > 0:
        with open(args[0], 'r') as f:
            settings = load(f)
        print settings
        signals = {pin_id: generate_signal(settings[pin_id]) for pin_id in settings}
        a = Ardulator(options.runtime, signals)
        # a.length = 50.0 # Another way of setting the secnario length
        # a.signals = signals # Updating the signals
        print a.run()
        print 'Total runtime:', a.length
    else:
        print "Please pass the program a configuration file."

if __name__ == '__main__':
    (options, args) = parser.parse_args()
    main(options, args)

