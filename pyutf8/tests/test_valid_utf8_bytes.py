from unittest import TestCase

import pyutf8

class TestValidUTF8Bytes(TestCase):
    def test_valid_utf8_bytes(self):
        valid_utf8_bytes = pyutf8.valid_utf8_bytes
        self.assertEquals(
           "invalid U+11ffff: ",
           valid_utf8_bytes("invalid U+11ffff: " + ''.join(map(chr, [244, 159, 191, 191]))))
        self.assertEquals(
           "U+10ffff: " + ''.join(map(chr, [244, 143, 191, 191])),
           valid_utf8_bytes("U+10ffff: " + ''.join(map(chr, [244, 143, 191, 191]))))
        self.assertEquals(
           "overlong 2-byte encoding (a): ",
           valid_utf8_bytes("overlong 2-byte encoding (a): " + ''.join(map(chr, [int('11000001', 2), int('10100001', 2)]))))
        self.assertEquals(
           "overlong 2-byte encoding (!): ",
           valid_utf8_bytes("overlong 2-byte encoding (!): " + ''.join(map(chr, [int('11000000', 2), int('10100001', 2)]))))
        self.assertEquals(
           "mu: " + ''.join(map(chr, [194, 181])),
           valid_utf8_bytes("mu: " + ''.join(map(chr, [194, 181]))))
        self.assertEquals(
           "bad coding bytes: ",
           valid_utf8_bytes("bad coding bytes: " + ''.join(map(chr, [int('10011111', 2), int('10111111', 2), int('11111111', 2)]))))
        self.assertEquals(
           "low surrogate (unpaired): ",
           valid_utf8_bytes("low surrogate (unpaired): " + ''.join(map(chr, [237, 176, 128]))))
        self.assertEquals(
           "high surrogate (unpaired): ",
           valid_utf8_bytes("high surrogate (unpaired): " + ''.join(map(chr, [237, 191, 191]))))
        self.assertEquals(
           "unicode snowman for you: " + ''.join(map(chr, [226, 152, 131])),
           valid_utf8_bytes("unicode snowman for you: " + ''.join(map(chr, [226, 152, 131]))))
        self.assertEquals(
           "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; (AISPW))",
           valid_utf8_bytes("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; ("
                              + ''.join(map(chr, [167, 65, 170, 186, 73, 83, 80, 166, 87, 186, 217, 41, 41]))))
