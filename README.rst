pyutf8 provides tools to efficiently deal with the validation
and cleanup of UTF-8 strings. The primary use case is "garbage in utf-8 out".

Usage::

    from pyutf8 import valid_utf8_bytes
    valid_utf8_bytes(u'\N{SNOWMAN}')
