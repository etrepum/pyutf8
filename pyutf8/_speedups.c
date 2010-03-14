/* -*- Mode: C; c-file-style: "python-new"; tab-width: 4; indent-tabs-mode: nil -*- */
#include "Python.h"
#include "structmember.h"
#if PY_VERSION_HEX < 0x02060000 && !defined(Py_TYPE)
#define Py_TYPE(ob)     (((PyObject*)(ob))->ob_type)
#endif
#if PY_VERSION_HEX < 0x02050000 && !defined(PY_SSIZE_T_MIN)
typedef int Py_ssize_t;
#define PY_SSIZE_T_MAX INT_MAX
#define PY_SSIZE_T_MIN INT_MIN
#define PyInt_FromSsize_t PyInt_FromLong
#define PyInt_AsSsize_t PyInt_AsLong
#endif

#ifdef __GNUC__
#define UNUSED __attribute__((__unused__))
#else
#define UNUSED
#endif

#ifdef Py_UNICODE_WIDE
#endif

static PyObject *
py_valid_utf8_bytes(PyObject *self UNUSED, PyObject *pystr);
static PyObject *
valid_utf8_bytes(PyObject *s);

PyDoc_STRVAR(pydoc_valid_utf8_bytes,
    "valid_utf8_bytes(basestring) -> str\n"
    "\n"
    "Return only the bytes in B that represent valid UTF-8. Uses "
    "the following algorithm: skip one byte if B does not "
    "follow UTF-8 syntax (a 1-4 byte encoding of some number), "
    "skip sequence of 2-4 bytes if it represents an overlong encoding "
    "or bad code point (surrogate U+D800 - U+DFFF or > U+10FFFF)."
);

static PyObject *
py_valid_utf8_bytes(PyObject *self UNUSED, PyObject *pystr)
{
    /* METH_O */
    PyObject *s;
    if (PyUnicode_Check(pystr)) {
        s = PyUnicode_AsUTF8String(pystr);
        if (s == NULL)
            return NULL;
    }
    else if (PyString_Check(pystr)) {
        Py_INCREF(pystr);
        s = pystr;
    }
    else {
        PyErr_Format(PyExc_TypeError,
                     "first argument must be a string, not %.80s",
                     Py_TYPE(pystr)->tp_name);
        return NULL;
    }
    return valid_utf8_bytes(s);
}

static PyObject *
valid_utf8_bytes(PyObject *s)
{
    Py_ssize_t i = 0;
    int c, c1, c2, c3, c4;
    PyObject *outstr = NULL;
    Py_ssize_t len = PyString_GET_SIZE(s);
    Py_ssize_t outidx = 0;
    char *outchars = NULL;
    char *chars = PyString_AS_STRING(s);
    /* Unallocated fast-loop, run through the string and hope that
       this is a no-op so that we can exit without allocating any memory */
    for (i=0; i < len; i++) {
        c1 = (unsigned char)chars[i];
        if (c1 < 0x80) {
            /* U+0000 - U+007F - 7 bits */
            continue;
        }
        if (i + 1 == len) {
            outidx = i;
            break;
        }
        c2 = (unsigned char)chars[i + 1];
        if (((c1 & 0xE0) == 0xC0) && ((c2 & 0xC0) == 0x80)) {
            /* U+0080 - U+07FF - 11 bits */
            i += 1;
            c = (((c1 & 0x1F) << 6) |
                 (c2 & 0x3F));
            if (c < 0x80) {
                /* Overlong encoding */
                outidx = i - 1;
                break;
            }
            else {
                continue;
            }
        }
        if (i + 2 == len) {
            outidx = i;
            break;
        }
        c3 = (unsigned char)chars[i + 2];
        if (((c1 & 0xF0) == 0xE0) &&
            ((c2 & 0xC0) == 0x80) &&
            ((c3 & 0xC0) == 0x80)) {
            /* U+0800 - U+FFFF - 16 bits */
            i += 2;
            c = (((((c1 & 0x0F) << 6) |
                   (c2 & 0x3F)) << 6) |
                 (c3 & 0x3f));
            if (((c < 0x800) || (c >= 0xD800 && c <= 0xDFFF))) {
                /* Overlong encoding or surrogate. */
                outidx = i - 2;
                break;
            }
            else {
                continue;
            }
        }
        if (i + 3 == len) {
            outidx = i;
            break;
        }
        c4 = (unsigned char)chars[i + 3];
        if (((c1 & 0xF8) == 0xF0) &&
            ((c2 & 0xC0) == 0x80) &&
            ((c3 & 0xC0) == 0x80) &&
            ((c4 & 0xC0) == 0x80)) {
            /* U+10000 - U+10FFFF - 21 bits */
            i += 3;
            c = (((((((c1 & 0x0F) << 6) |
                     (c2 & 0x3F)) << 6) |
                   (c3 & 0x3F)) << 6) |
                 (c4 & 0x3F));
            if ((c < 0x10000) || (c > 0x10FFFF)) {
                /* Overlong encoding or invalid code point. */
                outidx = i - 3;
                break;
            }
            else {
                continue;
            }
        }
        outidx = i;
        break;
    }
    if (i == len)
        return s;
    /* Just copy the whole string and allocate the whole length,
       we will have to realloc smaller at the end anyway.

       Would there be any advantage to Py_DECREF(s) at this point and
       read+write only outstr? It's possible to do this transform
       in-place.
    */
    outstr = PyString_FromStringAndSize(chars, len);
    if (outstr == NULL) {
        return NULL;
    }
    outchars = PyString_AS_STRING(outstr);
    /* Essentially the same loop as before but with all of the extra
       crap to copy chars to the new buffer */
    for (++i; i < len; i++) {
        c1 = (unsigned char)chars[i];
        if (c1 < 0x80) {
            /* U+0000 - U+007F - 7 bits */
            outchars[outidx++] = (char)c1;
            continue;
        }
        if (i + 1 == len) {
            continue;
        }
        c2 = (unsigned char)chars[i + 1];
        if (((c1 & 0xE0) == 0xC0) && ((c2 & 0xC0) == 0x80)) {
            /* U+0080 - U+07FF - 11 bits */
            i += 1;
            c = (((c1 & 0x1F) << 6) |
                 (c2 & 0x3F));
            if (c < 0x80) {
                /* Overlong encoding */
                continue;
            }
            else {
                outchars[outidx++] = (char)c1;
                outchars[outidx++] = (char)c2;
                continue;
            }
        }
        if (i + 2 == len) {
            continue;
        }
        c3 = (unsigned char)chars[i + 2];
        if (((c1 & 0xF0) == 0xE0) &&
            ((c2 & 0xC0) == 0x80) &&
            ((c3 & 0xC0) == 0x80)) {
            /* U+0800 - U+FFFF - 16 bits */
            i += 2;
            c = (((((c1 & 0x0F) << 6) |
                   (c2 & 0x3F)) << 6) |
                 (c3 & 0x3f));
            if (((c < 0x800) || (c >= 0xD800 && c <= 0xDFFF))) {
                /* Overlong encoding or surrogate. */
                continue;
            }
            else {
                outchars[outidx++] = (char)c1;
                outchars[outidx++] = (char)c2;
                outchars[outidx++] = (char)c3;
                continue;
            }
        }
        if (i + 3 == len) {
            continue;
        }
        c4 = (unsigned char)chars[i + 3];
        if (((c1 & 0xF8) == 0xF0) &&
            ((c2 & 0xC0) == 0x80) &&
            ((c3 & 0xC0) == 0x80) &&
            ((c4 & 0xC0) == 0x80)) {
            /* U+10000 - U+10FFFF - 21 bits */
            i += 3;
            c = (((((((c1 & 0x0F) << 6) |
                     (c2 & 0x3F)) << 6) |
                   (c3 & 0x3F)) << 6) |
                 (c4 & 0x3F));
            if ((c < 0x10000) || (c > 0x10FFFF)) {
                /* Overlong encoding or invalid code point. */
                continue;
            }
            else {
                outchars[outidx++] = (char)c1;
                outchars[outidx++] = (char)c2;
                outchars[outidx++] = (char)c3;
                outchars[outidx++] = (char)c4;
                continue;
            }
        }
        continue;
    }
    Py_DECREF(s);
    if (_PyString_Resize(&outstr, outidx) == -1) {
        return NULL;
    }
    return outstr;
}

static PyMethodDef speedups_methods[] = {
    {"valid_utf8_bytes",
        (PyCFunction)py_valid_utf8_bytes,
        METH_O,
        pydoc_valid_utf8_bytes},
    {NULL, NULL, 0, NULL}
};

PyDoc_STRVAR(module_doc,
"pyutf8 speedups\n");

void
init_speedups(void)
{
    PyObject *m;
    m = Py_InitModule3("_speedups", speedups_methods, module_doc);
}
