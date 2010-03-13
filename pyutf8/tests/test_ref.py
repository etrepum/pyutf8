from unittest import TestCase

from pyutf8.ref import str_skip_bytes, invalid_utf8_indexes


class TestRef(TestCase):
    def test_str_skip_bytes(self):
        self.assertEquals(
            "foo",
            str_skip_bytes("foo", []))
        self.assertEquals(
            "foobar",
            str_skip_bytes("foo bar", [3]))
        self.assertEquals(
            "foo",
            str_skip_bytes("foo bar", [3, 4, 5, 6]))
        self.assertEquals(
            "oo bar",
            str_skip_bytes("foo bar", [0]))

    def test_invalid_utf8_indexes(self):
        self.assertEquals(
            [],
            invalid_utf8_indexes([226, 152, 131]))
        self.assertEquals(
            [],
            invalid_utf8_indexes(map(ord, "unicode snowman for you: ") + [226, 152, 131]))
        self.assertEquals(
            [0],
            invalid_utf8_indexes([128])),
        self.assertEquals(
            [57, 59, 60, 64, 66, 67],
            invalid_utf8_indexes(map(ord, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; (") +
                [167, 65, 170, 186, 73, 83, 80, 166, 87, 186, 217, 41, 41]))
        
    