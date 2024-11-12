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

#include "extunsignedinteger.h"

ExtUnsignedInteger::ExtUnsignedInteger(const std::string & sName,
                                       OpenStatistic::StatisticNumeric<std::uint64_t> * pStatistic):
  sName_{sName},
  pStatistic_{pStatistic}
{}

static void
ExtUnsignedInteger_dealloc(ExtUnsignedInteger * self)
{
  self->~ExtUnsignedInteger();

  reinterpret_cast<PyObject*>(self)->ob_type->tp_free(reinterpret_cast<PyObject*>(self));
}

static PyObject *
ExtUnsignedInteger_repr(PyObject * self)
{
  ExtUnsignedInteger * pSelf{reinterpret_cast<ExtUnsignedInteger *>(self)};

  return PyUnicode_FromFormat("<ExtUnsignedInteger at %p %s>",
                              pSelf,
                              pSelf->sName_.c_str());
};

PyDoc_STRVAR(ExtUnsignedInteger_set_doc,
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
             "    foo_uint.set(42)\n"
             "\n");
static PyObject *
ExtUnsignedInteger_set(PyObject * self, PyObject *args)
{
  ExtUnsignedInteger * pSelf{reinterpret_cast<ExtUnsignedInteger *>(self)};

  std::uint64_t u64Value{};

  if(!PyArg_ParseTuple(args,
                       "K",
                       &u64Value))
    {
      return nullptr;
    }

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      *(pSelf->pStatistic_) = u64Value;
    }
  catch(std::exception & exp)
    {
      sError = exp.what();
    }

  Py_END_ALLOW_THREADS;

  if(!sError.empty())
    {
      PyErr_SetString(PyExc_RuntimeError,sError.c_str());
      return nullptr;
    }

  Py_INCREF(Py_None);

  return Py_None;
}

PyDoc_STRVAR(ExtUnsignedInteger_get_doc,
             "get(self)\n\n"
             "Gets the OpenStatistic statistic value.\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             "Example:\n"
             "    foo_uint.get()\n"
             "\n");
static PyObject *
ExtUnsignedInteger_get(PyObject * self, PyObject *args)
{
  ExtUnsignedInteger * pSelf{reinterpret_cast<ExtUnsignedInteger *>(self)};

  std::uint64_t u64Value{};

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      u64Value = pSelf->pStatistic_->get();
    }
  catch(std::exception & exp)
    {
      sError = exp.what();
    }

  Py_END_ALLOW_THREADS;

  if(!sError.empty())
    {
      PyErr_SetString(PyExc_RuntimeError,sError.c_str());
      return nullptr;
    }

  return PyLong_FromUnsignedLongLong(u64Value);
}


PyObject * ExtUnsignedInteger_iadd(PyObject * self, PyObject * obj)
{
  ExtUnsignedInteger * pSelf{reinterpret_cast<ExtUnsignedInteger *>(self)};

  std::uint64_t u64Value{};

  if(PyLong_Check(obj))
    {
      u64Value = PyLong_AsUnsignedLongLong(obj);

      if(PyErr_Occurred())
        {
          return nullptr;
        }
    }
  else
    {
      PyErr_SetString(PyExc_TypeError,
                      "addedend must be an unsigned integer");
      return nullptr;
    }

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      *(pSelf->pStatistic_) += u64Value;
    }
  catch(std::exception & exp)
    {
      sError = exp.what();
    }

  Py_END_ALLOW_THREADS;

  if(!sError.empty())
    {
      PyErr_SetString(PyExc_RuntimeError,sError.c_str());
      return nullptr;
    }

  Py_INCREF(self);

  return self;
}

PyObject * ExtUnsignedInteger_isub(PyObject * self, PyObject * obj)
{
  ExtUnsignedInteger * pSelf{reinterpret_cast<ExtUnsignedInteger *>(self)};

  std::uint64_t u64Value{};

  if(PyLong_Check(obj))
    {
      u64Value = PyLong_AsUnsignedLongLong(obj);

      if(PyErr_Occurred())
        {
          return nullptr;
        }
    }
  else
    {
      PyErr_SetString(PyExc_TypeError,
                      "subtrahend must be an unsigned integer");
      return nullptr;
    }

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      if(u64Value > pSelf->pStatistic_->get())
        {
          sError = "subtrahend cannot be more than unsigned statistic value";
        }
      else
        {
          *(pSelf->pStatistic_) -= u64Value;
        }
    }
  catch(std::exception & exp)
    {
      sError = exp.what();
    }

  Py_END_ALLOW_THREADS;

  if(!sError.empty())
    {
      PyErr_SetString(PyExc_RuntimeError,sError.c_str());
      return nullptr;
    }

  Py_INCREF(self);

  return self;
}

static PyObject *
ExtUnsignedInteger_str(PyObject * self)
{
  return PyObject_Str(ExtUnsignedInteger_get(self,nullptr));
};

static PyMethodDef ExtUnsignedInteger_methods[] =
  {
    {
      "set",
      reinterpret_cast<PyCFunction>(ExtUnsignedInteger_set),
      METH_VARARGS,
      ExtUnsignedInteger_set_doc,
    },

    {
      "get",
      reinterpret_cast<PyCFunction>(ExtUnsignedInteger_get),
      METH_NOARGS,
      ExtUnsignedInteger_get_doc,
    },

    {nullptr, nullptr, 0, nullptr}
  };

static PyNumberMethods ExtUnsignedInteger_number_methods =
  {
    nullptr, // binaryfunc nb_add
    nullptr, // binaryfunc nb_subtract
    nullptr, // binaryfunc nb_multiply
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
    ExtUnsignedInteger_iadd, // binaryfunc nb_inplace_add
    ExtUnsignedInteger_isub, // binaryfunc nb_inplace_subtract
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

PyDoc_STRVAR(ExtUnsignedInteger_type_doc,
             "OpenStatistic UnsignedInteger extension\n");
PyTypeObject ExtUnsignedIntegerType =
  {
    PyVarObject_HEAD_INIT(nullptr, 0)
    "UnsignedInteger",
    sizeof(ExtUnsignedInteger), // tp_basicsize
    0, // tp_itemsize
    reinterpret_cast<destructor>(ExtUnsignedInteger_dealloc),  // tp_dealloc
    0, // tp_print
    0, // tp_getattr
    0, // tp_setattr
    0, // tp_compare
    ExtUnsignedInteger_repr, // tp_repr
    &ExtUnsignedInteger_number_methods,// tp_as_number
    0, // tp_as_sequence
    0, // tp_as_mapping
    0, // tp_hash
    0, // tp_call
    ExtUnsignedInteger_str, // tp_str
    0, // tp_getattro
    0, // tp_setattro
    0, // tp_as_buffer
    Py_TPFLAGS_DEFAULT, // tp_flags
    ExtUnsignedInteger_type_doc, // tp_doc
    0, // tp_traverse
    0, // tp_clear
    0, // tp_richcompare
    0, // tp_weaklistoffset
    0, // tp_iter
    0, // tp_iternext
    ExtUnsignedInteger_methods, // tp_methods
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
ExtUnsignedInteger_FromStatisticNumeric(const std::string & sName,
                                        OpenStatistic::StatisticNumeric<std::uint64_t> * pStatistic)
{
  ExtUnsignedInteger* pExtUnsignedInteger =
    reinterpret_cast<ExtUnsignedInteger *>(ExtUnsignedIntegerType.tp_alloc(&ExtUnsignedIntegerType, 0));

  new (pExtUnsignedInteger) ExtUnsignedInteger{sName,pStatistic};

  return reinterpret_cast<PyObject *>(pExtUnsignedInteger);
}
