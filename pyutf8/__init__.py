r"""
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

def _toggle_speedups(enabled):
    global c_valid_utf8_bytes, valid_utf8_bytes
    if enabled:
        c_valid_utf8_bytes = _import_c_valid_utf8_bytes()
    else:
        c_valid_utf8_bytes = None
    valid_utf8_bytes = c_valid_utf8_bytes or py_valid_utf8_bytes
