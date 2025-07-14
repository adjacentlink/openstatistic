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

#include "extstring.h"

ExtString::ExtString(const std::string & sName,
                     OpenStatistic::StatisticNonNumeric<std::string> * pStatistic):
  sName_{sName},
  pStatistic_{pStatistic}
{}

static void
ExtString_dealloc(ExtString * self)
{
  self->~ExtString();

  reinterpret_cast<PyObject*>(self)->ob_type->tp_free(reinterpret_cast<PyObject*>(self));
}

static PyObject *
ExtString_repr(PyObject * self)
{
  ExtString * pSelf{reinterpret_cast<ExtString *>(self)};

  return PyUnicode_FromFormat("<ExtString at %p %s>",
                              pSelf,
                              pSelf->sName_.c_str());
};

PyDoc_STRVAR(ExtString_set_doc,
             "set(self,args)\n\n"
             "Sets the OpenStatistic statistic value.\n"
             "\n"
             "Args:\n"
             "    value (str): The value.\n\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             "Example:\n"
             "    foo_string.set('hello world')\n"
             "\n");
static PyObject *
ExtString_set(PyObject * self, PyObject *args)
{
  ExtString * pSelf{reinterpret_cast<ExtString *>(self)};

  const char * pzValue{};

  if(!PyArg_ParseTuple(args,
                       "s",
                       &pzValue))
    {
      return nullptr;
    }

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      *(pSelf->pStatistic_) = pzValue;
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


PyDoc_STRVAR(ExtString_get_doc,
             "get(self)\n\n"
             "Gets the OpenStatistic statistic value.\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             "Example:\n"
             "    foo_string.get()\n"
             "\n");
static PyObject *
ExtString_get(PyObject * self, PyObject *args)
{
  ExtString * pSelf{reinterpret_cast<ExtString *>(self)};

  std::string sError{};

  std::string sValue{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      sValue = pSelf->pStatistic_->get();
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
  const char * errors{};

  return PyUnicode_DecodeUTF8(sValue.c_str(),sValue.size(),errors);
}

static PyObject *
ExtString_str(PyObject * self)
{
  return PyObject_Str(ExtString_get(self,nullptr));
};

static PyMethodDef ExtString_methods[] =
  {
    {
      "set",
      reinterpret_cast<PyCFunction>(ExtString_set),
      METH_VARARGS,
      ExtString_set_doc,
    },

    {
      "get",
      reinterpret_cast<PyCFunction>(ExtString_get),
      METH_NOARGS,
      ExtString_get_doc,
    },

    {nullptr, nullptr, 0, nullptr}
  };


PyDoc_STRVAR(ExtString_type_doc,
             "OpenStatistic String extension\n");
PyTypeObject ExtStringType =
  {
    PyVarObject_HEAD_INIT(NULL, 0)
    "String",
    sizeof(ExtString), // tp_basicsize
    0, // tp_itemsize
    reinterpret_cast<destructor>(ExtString_dealloc), // tp_dealloc
    0, // tp_print
    0, // tp_getattr
    0, // tp_setattr
    0, // tp_compare
    ExtString_repr, // tp_repr
    0, // tp_as_number
    0, // tp_as_sequence
    0, // tp_as_mapping
    0, // tp_hash
    0, // tp_call
    ExtString_str, // tp_str
    0, // tp_getattro
    0, // tp_setattro
    0, // tp_as_buffer
    Py_TPFLAGS_DEFAULT, // tp_flags
    ExtString_type_doc, // tp_doc
    0, // tp_traverse
    0, // tp_clear
    0, // tp_richcompare
    0, // tp_weaklistoffset
    0, // tp_iter
    0, // tp_iternext
    ExtString_methods, // tp_methods
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
ExtString_FromStatisticNumeric(const std::string & sName,
                               OpenStatistic::StatisticNonNumeric<std::string> * pStatistic)
{
  ExtString * pExtString =
    reinterpret_cast<ExtString *>(ExtStringType.tp_alloc(&ExtStringType, 0));

  new (pExtString) ExtString{sName,pStatistic};

  return reinterpret_cast<PyObject *>(pExtString);
}
