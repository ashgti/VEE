#!/usr/bin/env python
import unittest

try:
    import vee
except ImportError:
    import sys
    sys.path.insert(0, '..')

from vee import Arduino
from vee.signals import Deterministic as d

class TestFailures(unittest.TestCase):
    def test_running(self):
        failing = Arduino()
        failing.length = 10
        failing.pins = [d('z')]

        # Run the sample script
        failing.run()
        self.assertEquals(failing.failures, 1)

if __name__ == '__main__':
    unittest.main()