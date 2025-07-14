/*
 * Copyright (c) 2024 - Adjacent Link LLC, Bridgewater, New Jersey
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of Adjacent Link LLC nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * See toplevel COPYING for more information.
 */

#include "extsignedinteger.h"

ExtSignedInteger::ExtSignedInteger(const std::string & sName,
                                   OpenStatistic::StatisticNumeric<std::int64_t> * pStatistic):
  sName_{sName},
  pStatistic_{pStatistic}
{}

static void
ExtSignedInteger_dealloc(ExtSignedInteger * self)
{
  self->~ExtSignedInteger();

  reinterpret_cast<PyObject*>(self)->ob_type->tp_free(reinterpret_cast<PyObject*>(self));
}

static PyObject *
ExtSignedInteger_repr(PyObject * self)
{
  ExtSignedInteger * pSelf{reinterpret_cast<ExtSignedInteger *>(self)};

  return PyUnicode_FromFormat("<ExtSignedInteger at %p %s>",
                              pSelf,
                              pSelf->sName_.c_str());
};

PyDoc_STRVAR(ExtSignedInteger_set_doc,
             "set(self,args)\n\n"
             "Sets the OpenStatistic statistic value.\n"
             "\n"
             "Args:\n"
             "    value (float): The value.\n\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             "Example:\n"
             "    foo_sint.set(-42)\n"
             "\n");
static PyObject *
ExtSignedInteger_set(PyObject * self, PyObject *args)
{
  ExtSignedInteger * pSelf{reinterpret_cast<ExtSignedInteger *>(self)};

  std::int64_t i64Value{};

  if(!PyArg_ParseTuple(args,
                       "L",
                       &i64Value))
    {
      return nullptr;
    }

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      *(pSelf->pStatistic_) = i64Value;
    }
  catch(std::exception & exp)
    {
      sError = exp.what();
    }

  Py_END_ALLOW_THREADS;

  if(!sError.empty())
    {
      PyErr_SetString(PyExc_RuntimeError,
                      sError.c_str());
      return nullptr;
    }

  Py_INCREF(Py_None);

  return Py_None;
}

PyDoc_STRVAR(ExtSignedInteger_get_doc,
             "get(self)\n\n"
             "Gets the OpenStatistic statistic value.\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             "Example:\n"
             "    foo_sint.get()\n"
             "\n");
static PyObject *
ExtSignedInteger_get(PyObject * self, PyObject *args)
{
  ExtSignedInteger * pSelf{reinterpret_cast<ExtSignedInteger *>(self)};

  std::int64_t i64Value{};

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      i64Value = pSelf->pStatistic_->get();
    }
  catch(std::exception & exp)
    {
      sError = exp.what();
    }

  Py_END_ALLOW_THREADS;

  if(!sError.empty())
    {
      PyErr_SetString(PyExc_RuntimeError,
                      sError.c_str());
      return nullptr;
    }

  return PyLong_FromLongLong(i64Value);
}


PyObject * ExtSignedInteger_iadd(PyObject * self, PyObject * obj)
{
  ExtSignedInteger * pSelf{reinterpret_cast<ExtSignedInteger *>(self)};

  std::int64_t i64Value{};

  if(PyLong_Check(obj))
    {
      i64Value = PyLong_AsLongLong(obj);

      if(PyErr_Occurred())
        {
          return nullptr;
        }
    }
  else
    {
      PyErr_SetString(PyExc_TypeError,
                      "addedend must be an integer");
      return nullptr;
    }

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      *(pSelf->pStatistic_) += i64Value;
    }
  catch(std::exception & exp)
    {
      sError = exp.what();
    }

  Py_END_ALLOW_THREADS;

  if(!sError.empty())
    {
      PyErr_SetString(PyExc_RuntimeError,
                      sError.c_str());
      return nullptr;
    }

  Py_INCREF(self);

  return self;
}

PyObject * ExtSignedInteger_isub(PyObject * self, PyObject * obj)
{
  ExtSignedInteger * pSelf{reinterpret_cast<ExtSignedInteger *>(self)};

  std::int64_t i64Value{};

  if(PyLong_Check(obj))
    {
      i64Value = PyLong_AsLongLong(obj);

      if(PyErr_Occurred())
        {
          return nullptr;
        }
    }
  else
    {
      PyErr_SetString(PyExc_TypeError,
                      "subtrahend must be an integer");
      return nullptr;
    }

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      *(pSelf->pStatistic_) -= i64Value;
    }
  catch(std::exception & exp)
    {
      sError = exp.what();
    }

  Py_END_ALLOW_THREADS;

  if(!sError.empty())
    {
      PyErr_SetString(PyExc_RuntimeError,
                      sError.c_str());
      return nullptr;
    }

  Py_INCREF(self);

  return self;
}

static PyObject *
ExtSignedInteger_str(PyObject * self)
{
  return PyObject_Str(ExtSignedInteger_get(self,nullptr));
};

static PyMethodDef ExtSignedInteger_methods[] =
  {
    {
      "set",
      reinterpret_cast<PyCFunction>(ExtSignedInteger_set),
      METH_VARARGS,
      ExtSignedInteger_set_doc,
    },

    {
      "get",
      reinterpret_cast<PyCFunction>(ExtSignedInteger_get),
      METH_NOARGS,
      ExtSignedInteger_get_doc,
    },

    {nullptr, nullptr, 0, nullptr}
  };

static PyNumberMethods ExtSignedInteger_number_methods =
  {
    nullptr, // binaryfunc nb_add
    nullptr, //  binaryfunc nb_subtract
    nullptr, //  binaryfunc nb_multiply
    nullptr, // binaryfunc nb_remainder
    nullptr, // binaryfunc nb_divmod
    nullptr, // ternaryfunc nb_power
    nullptr, // unaryfunc nb_negative
    nullptr, // unaryfunc nb_positive
    nullptr, // unaryfunc nb_absolute
    nullptr, // inquiry nb_bool
    nullptr, // unaryfunc nb_invert
    nullptr, // binaryfunc nb_lshift
    nullptr, // binaryfunc nb_rshift
    nullptr, // binaryfunc nb_and
    nullptr, // binaryfunc nb_xor
    nullptr, // binaryfunc nb_or
    nullptr, // unaryfunc nb_int
    nullptr, // void *nb_reserved
    nullptr, // unaryfunc nb_float
    ExtSignedInteger_iadd, // binaryfunc nb_inplace_add
    ExtSignedInteger_isub, // binaryfunc nb_inplace_subtract
    nullptr, // binaryfunc nb_inplace_multiply
    nullptr, // binaryfunc nb_inplace_remainder
    nullptr, // ternaryfunc nb_inplace_power
    nullptr, // binaryfunc nb_inplace_lshift
    nullptr, // binaryfunc nb_inplace_rshift
    nullptr, // binaryfunc nb_inplace_and
    nullptr, // binaryfunc nb_inplace_xor
    nullptr, // binaryfunc nb_inplace_or
    nullptr, // binaryfunc nb_floor_divide
    nullptr, // binaryfunc nb_true_divide
    nullptr, // binaryfunc nb_inplace_floor_divide
    nullptr, // binaryfunc nb_inplace_true_divide
    nullptr, // unaryfunc nb_index
    nullptr, // binaryfunc nb_matrix_multiply
    nullptr, // binaryfunc nb_inplace_matrix_multiply
  };

PyDoc_STRVAR(ExtSignedInteger_type_doc,
             "OpenStatistic SignedInteger extension\n");
PyTypeObject ExtSignedIntegerType =
  {
    PyVarObject_HEAD_INIT(NULL, 0)
    "SignedInteger",
    sizeof(ExtSignedInteger), // tp_basicsize
    0, // tp_itemsize
    reinterpret_cast<destructor>(ExtSignedInteger_dealloc),  // tp_dealloc
    0, // tp_vectorcall_offsetp
    0, // tp_getattr
    0, // tp_setattr
    0, // tp_compare
    ExtSignedInteger_repr, // tp_repr
    &ExtSignedInteger_number_methods, // tp_as_number
    0, // tp_as_sequence
    0, // tp_as_mapping
    0, // tp_hash
    0, // tp_call
    ExtSignedInteger_str, // tp_str
    0, // tp_getattro
    0, // tp_setattro
    0, // tp_as_buffer
    Py_TPFLAGS_DEFAULT, // tp_flags
    ExtSignedInteger_type_doc, // tp_doc
    0, // tp_traverse
    0, // tp_clear
    0, // tp_richcompare
    0, // tp_weaklistoffset
    0, // tp_iter
    0, // tp_iternext
    ExtSignedInteger_methods, // tp_methods
    0, // tp_members
    0, // tp_getset
    0, // tp_base
    0, // tp_dict
    0, // tp_descr_get
    0, // tp_descr_set
    0, // tp_dictoffset
    0, // tp_init
    0, // tp_alloc
    0, // tp_new
  };

PyObject *
ExtSignedInteger_FromStatisticNumeric(const std::string & sName,
                                      OpenStatistic::StatisticNumeric<std::int64_t> * pStatistic)
{
  ExtSignedInteger* pExtSignedInteger =
    reinterpret_cast<ExtSignedInteger *>(ExtSignedIntegerType.tp_alloc(&ExtSignedIntegerType, 0));

  new (pExtSignedInteger) ExtSignedInteger{sName,pStatistic};

  return  reinterpret_cast<PyObject *>(pExtSignedInteger);
}
