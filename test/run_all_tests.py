#!/usr/bin/env python
import os
import sys
import unittest

if __name__ == '__main__':
    # Add the path to the library
    sys.path.insert(0, '..')

    mainsuite = unittest.TestSuite()
    for modulename in [os.path.splitext(x)[0]
        for x in os.listdir('.')
            if x != __file__ and x.startswith("test") and x.endswith(".py")
    ]:
        try:
            module = __import__(modulename)
        except ImportError:
            print "skipping %s" % modulename
        else:
            testsuite = unittest.findTestCases(module)
            print "found %s tests in %r" % (testsuite.countTestCases(), modulename)
            mainsuite.addTest(testsuite)

    verbosity = 1
    if '-v' in sys.argv[1:]:
        verbosity = 2

    # run the collected tests
    testRunner = unittest.TextTestRunner(verbosity=verbosity)
    #~ testRunner = unittest.ConsoleTestRunner(verbosity=verbosity)
    result = testRunner.run(mainsuite)

    # set exit code accordingly to test results
    sys.exit(not result.wasSuccessful())
