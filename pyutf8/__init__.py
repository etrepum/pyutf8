r"""pyutf8 provides tools to efficiently deal with the validation
and cleanup of UTF-8 strings. The primary use case is "garbage in utf-8 out".

Usage::

    >>> from pyutf8 import valid_utf8_bytes
    >>> SNOWMAN = u'\N{SNOWMAN}'
    >>> SNOWMAN_BYTES = SNOWMAN.encode('utf-8')
    >>> valid_utf8_bytes(SNOWMAN) == SNOWMAN_BYTES
    True
    >>> valid_utf8_bytes(SNOWMAN_BYTES) == SNOWMAN_BYTES
    True
    >>> valid_utf8_bytes('\xff' + SNOWMAN_BYTES + '\xff') == SNOWMAN_BYTES
    True
"""
__version__ = '0.1'
__all__ = ['valid_utf8_bytes']
__author__ = 'Bob Ippolito <bob@redivi.com>'

from ref import valid_utf8_bytes as py_valid_utf8_bytes
def _import_c_valid_utf8_bytes():
    try:
        from pyutf8._speedups import valid_utf8_bytes
        return valid_utf8_bytes
    except ImportError:
        return None
c_valid_utf8_bytes = _import_c_valid_utf8_bytes()

valid_utf8_bytes = None
def _toggle_speedups(enabled):
    global c_valid_utf8_bytes, valid_utf8_bytes
    if enabled:
        c_valid_utf8_bytes = _import_c_valid_utf8_bytes()
    else:
        c_valid_utf8_bytes = None
    valid_utf8_bytes = c_valid_utf8_bytes or py_valid_utf8_bytes
_toggle_speedups(True)
