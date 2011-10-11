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

def main(options, args):
    if len(args) > 0:
        with open(args[0], 'r') as f:
            settings = load(f)
        print settings
        signals = {pin_id: generate_signal(settings[pin_id]) for pin_id in settings}
        print signals
        a = Ardulator(100, signals)
        # a.length = 50.0
        # a.signals = signals
        print a.run(25)
        print a.run()
    else:
        print "Please pass the program a configuration file."

if __name__ == '__main__':
    (options, args) = parser.parse_args()
    main(options, args)

