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
    PyErr_Format(PyExc_TypeError,
		 "first argument must be a string, not %.80s",
		 Py_TYPE(pystr)->tp_name);
    return NULL;
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
