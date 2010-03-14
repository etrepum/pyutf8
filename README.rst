pyutf8 provides tools to efficiently deal with the validation
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
