//-----------------------------------------------------------------------------
// Low level CRC functions for use by crcmod.  This version is the C
// implementation that corresponds to the Python module _crcfunpy.  This module
// will be used by crcmod if it is built for the target platform.  Otherwise,
// the Python module is used.
//
// Copyright (c) 2010  Raymond L. Buvel
// Copyright (c) 2010  Craig McQueen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#define PY_SSIZE_T_CLEAN
#include <Python.h>

// Note: the type declarations are set up to work on 32-bit and 64-bit
// platforms using the GNU C compiler.  They may need to be adjusted for other
// platforms.

// Define a few types to make it easier to port to other platforms.
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

// Define some macros for the data format strings.  The INPUT strings are for
// decoding the input parameters to the function which are (data, crc, table).

#define INPUT8 "OBs#"
#define INPUT16 "OHs#"
#define INPUT32 "OIs#"
#define INPUT64 "OKs#"

// The following macro is taken from hashlib.h in the Python 3.1 code,
// providing "Common code for use by all hashlib related modules".

// Given a PyObject* obj, fill in the Py_buffer* viewp with the result
// of PyObject_GetBuffer.  Sets an exception and issues a return NULL
// on any errors.
#define GET_BUFFER_VIEW_OR_ERROUT(obj, viewp) do { \
        if (PyUnicode_Check((obj))) { \
            PyErr_SetString(PyExc_TypeError, \
                            "Strings must be encoded before calculating a CRC");\
            return NULL; \
        } \
        if (!PyObject_CheckBuffer((obj))) { \
            PyErr_SetString(PyExc_TypeError, \
                            "object supporting the buffer API required"); \
            return NULL; \
        } \
        if (PyObject_GetBuffer((obj), (viewp), PyBUF_SIMPLE) == -1) { \
            return NULL; \
        } \
        if ((viewp)->ndim > 1) { \
            PyErr_SetString(PyExc_BufferError, \
                            "Buffer must be single dimension"); \
            PyBuffer_Release((viewp)); \
            return NULL; \
        } \
    } while(0);

// Define some macros that extract the specified byte from an integral value in
// what should be a platform independent manner.
#define BYTE0(x) ((UINT8)(x))
#define BYTE1(x) ((UINT8)((x) >> 8))
#define BYTE2(x) ((UINT8)((x) >> 16))
#define BYTE3(x) ((UINT8)((x) >> 24))
#define BYTE7(x) ((UINT8)((x) >> 56))

//-----------------------------------------------------------------------------
// Compute a 8-bit crc over the input data.
// Inputs:
//   data - string containing the data
//   crc - unsigned integer containing the initial crc
//   table - string containing the 8-bit table corresponding to the generator
//           polynomial.
// Returns:
//   crc - unsigned integer containing the resulting crc

static PyObject*
_crc8(PyObject* self, PyObject* args)
{
    PyObject *obj;
    Py_buffer buf;
    UINT8 crc;
    UINT8* data;
    Py_ssize_t dataLen;
    UINT8* table;
    Py_ssize_t tableLen;

    if (!PyArg_ParseTuple(args, INPUT8, &obj, &crc,
                            &table, &tableLen))
    {
        return NULL;
    }

    if (tableLen != 256)
    {
        PyErr_SetString(PyExc_ValueError, "invalid CRC table");
        return NULL;
    }

    GET_BUFFER_VIEW_OR_ERROUT(obj, &buf);
    data = buf.buf;
    dataLen = buf.len;

    while (dataLen--)
    {
        crc = table[*data ^ crc];
        data++;
    }

    PyBuffer_Release(&buf);

    return PyLong_FromLong((long)crc);
}

//-----------------------------------------------------------------------------
// Compute a 8-bit crc over the input data.  The data stream is bit reversed
// during the computation.
// Inputs:
//   data - string containing the data
//   crc - unsigned integer containing the initial crc
//   table - string containing the 8-bit table corresponding to the generator
//           polynomial.
// Returns:
//   crc - unsigned integer containing the resulting crc

static PyObject*
_crc8r(PyObject* self, PyObject* args)
{
    PyObject *obj;
    Py_buffer buf;
    UINT8 crc;
    UINT8* data;
    Py_ssize_t dataLen;
    UINT8* table;
    Py_ssize_t tableLen;

    if (!PyArg_ParseTuple(args, INPUT8, &obj, &crc,
                            &table, &tableLen))
    {
        return NULL;
    }

    if (tableLen != 256)
    {
        PyErr_SetString(PyExc_ValueError, "invalid CRC table");
        return NULL;
    }

    GET_BUFFER_VIEW_OR_ERROUT(obj, &buf);
    data = buf.buf;
    dataLen = buf.len;

    while (dataLen--)
    {
        crc = table[*data ^ crc];
        data++;
    }

    PyBuffer_Release(&buf);

    return PyLong_FromLong((long)crc);
}

//-----------------------------------------------------------------------------
// Compute a 16-bit crc over the input data.
// Inputs:
//   data - string containing the data
//   crc - unsigned integer containing the initial crc
//   table - string containing the 16-bit table corresponding to the generator
//           polynomial.
// Returns:
//   crc - unsigned integer containing the resulting crc

static PyObject*
_crc16(PyObject* self, PyObject* args)
{
    PyObject *obj;
    Py_buffer buf;
    UINT16 crc;
    UINT8* data;
    Py_ssize_t dataLen;
    UINT16* table;
    Py_ssize_t tableLen;

    if (!PyArg_ParseTuple(args, INPUT16, &obj, &crc,
                            &table, &tableLen))
    {
        return NULL;
    }

    if (tableLen != 256*2)
    {
        PyErr_SetString(PyExc_ValueError, "invalid CRC table");
        return NULL;
    }

    GET_BUFFER_VIEW_OR_ERROUT(obj, &buf);
    data = buf.buf;
    dataLen = buf.len;

    while (dataLen--)
    {
        crc = table[*data ^ BYTE1(crc)] ^ (crc << 8);
        data++;
    }

    PyBuffer_Release(&buf);

    return PyLong_FromLong((long)crc);
}

//-----------------------------------------------------------------------------
// Compute a 16-bit crc over the input data.  The data stream is bit reversed
// during the computation.
// Inputs:
//   data - string containing the data
//   crc - unsigned integer containing the initial crc
//   table - string containing the 16-bit table corresponding to the generator
//           polynomial.
// Returns:
//   crc - unsigned integer containing the resulting crc

static PyObject*
_crc16r(PyObject* self, PyObject* args)
{
    PyObject *obj;
    Py_buffer buf;
    UINT16 crc;
    UINT8* data;
    Py_ssize_t dataLen;
    UINT16* table;
    Py_ssize_t tableLen;

    if (!PyArg_ParseTuple(args, INPUT16, &obj, &crc,
                            &table, &tableLen))
    {
        return NULL;
    }

    if (tableLen != 256*2)
    {
        PyErr_SetString(PyExc_ValueError, "invalid CRC table");
        return NULL;
    }

    GET_BUFFER_VIEW_OR_ERROUT(obj, &buf);
    data = buf.buf;
    dataLen = buf.len;

    while (dataLen--)
    {
        crc = table[*data ^ BYTE0(crc)] ^ (crc >> 8);
        data++;
    }

    PyBuffer_Release(&buf);

    return PyLong_FromLong((long)crc);
}

//-----------------------------------------------------------------------------
// Compute a 24-bit crc over the input data.
// Inputs:
//   data - string containing the data
//   crc - unsigned integer containing the initial crc
//   table - string containing the 24-bit table corresponding to the generator
//           polynomial.
// Returns:
//   crc - unsigned integer containing the resulting crc

static PyObject*
_crc24(PyObject* self, PyObject* args)
{
    PyObject *obj;
    Py_buffer buf;
    UINT32 crc;
    UINT8* data;
    Py_ssize_t dataLen;
    UINT32* table;
    Py_ssize_t tableLen;

    if (!PyArg_ParseTuple(args, INPUT32, &obj, &crc,
                            &table, &tableLen))
    {
        return NULL;
    }

    if (tableLen != 256*4)
    {
        PyErr_SetString(PyExc_ValueError, "invalid CRC table");
        return NULL;
    }

    GET_BUFFER_VIEW_OR_ERROUT(obj, &buf);
    data = buf.buf;
    dataLen = buf.len;

    while (dataLen--)
    {
        crc = table[*data ^ BYTE2(crc)] ^ (crc << 8);
        data++;
    }

    PyBuffer_Release(&buf);

    return PyLong_FromLong((long)(crc & 0xFFFFFFU));
}

//-----------------------------------------------------------------------------
// Compute a 24-bit crc over the input data.  The data stream is bit reversed
// during the computation.
// Inputs:
//   data - string containing the data
//   crc - unsigned integer containing the initial crc
//   table - string containing the 24-bit table corresponding to the generator
//           polynomial.
// Returns:
//   crc - unsigned integer containing the resulting crc

static PyObject*
_crc24r(PyObject* self, PyObject* args)
{
    PyObject *obj;
    Py_buffer buf;
    UINT32 crc;
    UINT8* data;
    Py_ssize_t dataLen;
    UINT32* table;
    Py_ssize_t tableLen;

    if (!PyArg_ParseTuple(args, INPUT32, &obj, &crc,
                            &table, &tableLen))
    {
        return NULL;
    }

    if (tableLen != 256*4)
    {
        PyErr_SetString(PyExc_ValueError, "invalid CRC table");
        return NULL;
    }

    GET_BUFFER_VIEW_OR_ERROUT(obj, &buf);
    data = buf.buf;
    dataLen = buf.len;

    crc = crc & 0xFFFFFFU;
    while (dataLen--)
    {
        crc = table[*data ^ BYTE0(crc)] ^ (crc >> 8);
        data++;
    }

    PyBuffer_Release(&buf);

    return PyLong_FromLong((long)crc);
}

//-----------------------------------------------------------------------------
// Compute a 32-bit crc over the input data.
// Inputs:
//   data - string containing the data
//   crc - unsigned integer containing the initial crc
//   table - string containing the 32-bit table corresponding to the generator
//           polynomial.
// Returns:
//   crc - unsigned integer containing the resulting crc

static PyObject*
_crc32(PyObject* self, PyObject* args)
{
    PyObject *obj;
    Py_buffer buf;
    UINT32 crc;
    UINT8* data;
    Py_ssize_t dataLen;
    UINT32* table;
    Py_ssize_t tableLen;

    if (!PyArg_ParseTuple(args, INPUT32, &obj, &crc,
                            &table, &tableLen))
    {
        return NULL;
    }

    if (tableLen != 256*4)
    {
        PyErr_SetString(PyExc_ValueError, "invalid CRC table");
        return NULL;
    }

    GET_BUFFER_VIEW_OR_ERROUT(obj, &buf);
    data = buf.buf;
    dataLen = buf.len;

    while (dataLen--)
    {
        crc = table[*data ^ BYTE3(crc)] ^ (crc << 8);
        data++;
    }

    PyBuffer_Release(&buf);

    return PyLong_FromUnsignedLong(crc);
}

//-----------------------------------------------------------------------------
// Compute a 32-bit crc over the input data.  The data stream is bit reversed
// during the computation.
// Inputs:
//   data - string containing the data
//   crc - unsigned integer containing the initial crc
//   table - string containing the 32-bit table corresponding to the generator
//           polynomial.
// Returns:
//   crc - unsigned integer containing the resulting crc

static PyObject*
_crc32r(PyObject* self, PyObject* args)
{
    PyObject *obj;
    Py_buffer buf;
    UINT32 crc;
    UINT8* data;
    Py_ssize_t dataLen;
    UINT32* table;
    Py_ssize_t tableLen;

    if (!PyArg_ParseTuple(args, INPUT32, &obj, &crc,
                            &table, &tableLen))
    {
        return NULL;
    }

    if (tableLen != 256*4)
    {
        PyErr_SetString(PyExc_ValueError, "invalid CRC table");
        return NULL;
    }

    GET_BUFFER_VIEW_OR_ERROUT(obj, &buf);
    data = buf.buf;
    dataLen = buf.len;

    while (dataLen--)
    {
        crc = table[*data ^ BYTE0(crc)] ^ (crc >> 8);
        data++;
    }

    PyBuffer_Release(&buf);

    return PyLong_FromUnsignedLong(crc);
}

//-----------------------------------------------------------------------------
// Compute a 64-bit crc over the input data.
// Inputs:
//   data - string containing the data
//   crc - unsigned integer containing the initial crc
//   table - string containing the 64-bit table corresponding to the generator
//           polynomial.
// Returns:
//   crc - unsigned integer containing the resulting crc

static PyObject*
_crc64(PyObject* self, PyObject* args)
{
    PyObject *obj;
    Py_buffer buf;
    UINT64 crc;
    UINT8* data;
    Py_ssize_t dataLen;
    UINT64* table;
    Py_ssize_t tableLen;

    if (!PyArg_ParseTuple(args, INPUT64, &obj, &crc,
                            &table, &tableLen))
    {
        return NULL;
    }

    if (tableLen != 256*8)
    {
        PyErr_SetString(PyExc_ValueError, "invalid CRC table");
        return NULL;
    }

    GET_BUFFER_VIEW_OR_ERROUT(obj, &buf);
    data = buf.buf;
    dataLen = buf.len;

    while (dataLen--)
    {
        crc = table[*data ^ BYTE7(crc)] ^ (crc << 8);
        data++;
    }

    PyBuffer_Release(&buf);

    return PyLong_FromUnsignedLongLong(crc);
}

//-----------------------------------------------------------------------------
// Compute a 64-bit crc over the input data.  The data stream is bit reversed
// during the computation.
// Inputs:
//   data - string containing the data
//   crc - unsigned integer containing the initial crc
//   table - string containing the 64-bit table corresponding to the generator
//           polynomial.
// Returns:
//   crc - unsigned integer containing the resulting crc

static PyObject*
_crc64r(PyObject* self, PyObject* args)
{
    PyObject *obj;
    Py_buffer buf;
    UINT64 crc;
    UINT8* data;
    Py_ssize_t dataLen;
    UINT64* table;
    Py_ssize_t tableLen;

    if (!PyArg_ParseTuple(args, INPUT64, &obj, &crc,
                            &table, &tableLen))
    {
        return NULL;
    }

    if (tableLen != 256*8)
    {
        PyErr_SetString(PyExc_ValueError, "invalid CRC table");
        return NULL;
    }

    GET_BUFFER_VIEW_OR_ERROUT(obj, &buf);
    data = buf.buf;
    dataLen = buf.len;

    while (dataLen--)
    {
        crc = table[*data ^ BYTE0(crc)] ^ (crc >> 8);
        data++;
    }

    PyBuffer_Release(&buf);

    return PyLong_FromUnsignedLongLong(crc);
}

//-----------------------------------------------------------------------------
static PyMethodDef methodTable[] = {
{"_crc8", _crc8, METH_VARARGS},
{"_crc8r", _crc8r, METH_VARARGS},
{"_crc16", _crc16, METH_VARARGS},
{"_crc16r", _crc16r, METH_VARARGS},
{"_crc24", _crc24, METH_VARARGS},
{"_crc24r", _crc24r, METH_VARARGS},
{"_crc32", _crc32, METH_VARARGS},
{"_crc32r", _crc32r, METH_VARARGS},
{"_crc64", _crc64, METH_VARARGS},
{"_crc64r", _crc64r, METH_VARARGS},
{NULL, NULL}
};

//-----------------------------------------------------------------------------
static struct PyModuleDef moduleDef = {
   PyModuleDef_HEAD_INIT,
   "_crcfunext",  // name of module
   NULL,          // module documentation, may be NULL
   -1,            // size of per-interpreter state of the module,
                  // or -1 if the module keeps state in global variables.
   methodTable
};

//-----------------------------------------------------------------------------
PyMODINIT_FUNC
PyInit__crcfunext(void)
{
    if ((sizeof(UINT8) != 1) || (sizeof(UINT16) != 2) ||
        (sizeof(UINT32) != 4) || (sizeof(UINT64) != 8))
    {
        Py_FatalError("crcfunext: One of the data types is invalid");
    }

    PyObject* mod = PyModule_Create(&moduleDef);
#ifdef Py_GIL_DISABLED
    PyUnstable_Module_SetGIL(mod, Py_MOD_GIL_NOT_USED);
#endif
    return mod;
}
