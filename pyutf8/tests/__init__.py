import unittest
import doctest


class OptionalExtensionTestSuite(unittest.TestSuite):
    def run(self, result):
        import pyutf8
        run = unittest.TestSuite.run
        run(self, result)
        pyutf8._toggle_speedups(False)
        run(self, result)
        pyutf8._toggle_speedups(True)
        return result


def additional_tests(suite=None):
    import pyutf8
    import pyutf8.ref
    if suite is None:
        suite = unittest.TestSuite()
    for mod in (pyutf8, pyutf8.ref):
        suite.addTest(doctest.DocTestSuite(mod))
    suite.addTest(doctest.DocFileSuite('../../README.rst'))
    return suite


def all_tests_suite():
    suite = unittest.TestLoader().loadTestsFromNames([
        'pyutf8.tests.test_ref',
        'pyutf8.tests.test_valid_utf8_bytes',
    ])
    suite = additional_tests(suite)
    return OptionalExtensionTestSuite([suite])


def main():
    runner = unittest.TextTestRunner()
    suite = all_tests_suite()
    runner.run(suite)

if __name__ == '__main__':
    import os
    import sys
    sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
    main()
