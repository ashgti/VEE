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
    print options
    print args
    with open(args[0], 'r') as f:
        settings = load(f)
    print settings
    for pin_id in settings:
        print 'pinid:', pin_id
        print 'type:', validateSignalType(settings[pin_id]['type'])
    a = Ardulator()
    a.length = 50.0
    
    a.signals.append(SquareWave(5, 2.0, 1.0))
    
    print a.run(25)
    print a.run()

if __name__ == '__main__':
    (options, args) = parser.parse_args()
    main(options, args)

