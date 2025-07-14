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

#include "exttable.h"
#include "pyobjectutils.h"

ExtTable::ExtTable(const std::string & sName,
                   OpenStatistic::Table<Py_hash_t> * pTable,
                   PyObject * pClearCallableObject):
  sName_{sName},
  pTable_{pTable},
  pClearCallableObject_{pClearCallableObject}{}

static void
ExtTable_dealloc(ExtTable * self)
{
  self->~ExtTable();

  reinterpret_cast<PyObject*>(self)->ob_type->tp_free(reinterpret_cast<PyObject*>(self));
}

static PyObject *
ExtTable_repr(PyObject * self)
{
  ExtTable * pSelf{reinterpret_cast<ExtTable *>(self)};

  return PyUnicode_FromFormat("<ExtTable at %p %s>",
                              pSelf,
                              pSelf->sName_.c_str());
};

PyDoc_STRVAR(ExtTable_add_row_doc,
             "add_row(self,args)\n\n"
             "Adds an OpenStatistic table row.\n"
             "\n"
             "Args:\n"
             "    key (obj): A hashable key object.\n"
             "    values (tuple|list of (int|bool|double|str)): Row cell\n"
             "      values.\n\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             "Example:\n"
             "    foor_table.add_row(100,\n"
             "                       (80,'na'))\n"
             "\n");
static PyObject *
ExtTable_add_row(PyObject * self, PyObject *args)
{
  ExtTable * pSelf{reinterpret_cast<ExtTable *>(self)};

  PyObject * pObjectKey{};
  PyObject * pObjectValues{};

  if(!PyArg_ParseTuple(args,
                       "OO",
                       &pObjectKey,
                       &pObjectValues))
    {
      return nullptr;
    }

  auto hash = PyObject_Hash(pObjectKey);

  if(hash == -1)
    {
      PyErr_SetString(PyExc_RuntimeError,
                      "key object is not hashable");

      return nullptr;
    }

  std::vector<OpenStatistic::Any> anys{};

  try
    {
      PyObject_to_any_container(pObjectValues,anys);
    }
  catch(std::exception & exp)
    {
      PyErr_SetString(PyExc_RuntimeError,
                      exp.what());
      return nullptr;
    }

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      pSelf->pTable_->addRow(hash,anys);
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

PyDoc_STRVAR(ExtTable_delete_row_doc,
             "delete_row(self,args)\n\n"
             "Deletes an OpenStatistic table row.\n"
             "\n"
             "Args:\n"
             "    key (obj): A hashable key object.\n\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             "Example:\n"
             "    foor_table.dlete_row(100)\n"
             "\n");
static PyObject *
ExtTable_delete_row(PyObject * self, PyObject *args)
{
  ExtTable * pSelf{reinterpret_cast<ExtTable *>(self)};

  PyObject * pObjectKey{};

  if(!PyArg_ParseTuple(args,
                       "O",
                       &pObjectKey))
    {
      return nullptr;
    }

  auto hash = PyObject_Hash(pObjectKey);

  if(hash == -1)
    {
      PyErr_SetString(PyExc_RuntimeError,
                      "key object is not hashable");

      return nullptr;
    }

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      pSelf->pTable_->deleteRow(hash);
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


PyDoc_STRVAR(ExtTable_set_row_doc,
             "sets_row(self,args)\n\n"
             "Sets (replaces) an OpenStatistic table row.\n"
             "\n"
             "Args:\n"
             "    key (obj): A hashable key object.\n"
             "    values (tuple|list of (int|bool|double|str)): Row cell\n"
             "      values.\n\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             "Example:\n"
             "    foor_table.set_row(100,\n"
             "                       (80,'na'))\n"
             "\n");
static PyObject *
ExtTable_set_row(PyObject * self, PyObject *args)
{
  ExtTable * pSelf{reinterpret_cast<ExtTable *>(self)};

  PyObject * pObjectKey{};

  PyObject * pObjectValues{};

  if(!PyArg_ParseTuple(args,
                       "OO",
                       &pObjectKey,
                       &pObjectValues))
    {
      return nullptr;
    }

  auto hash = PyObject_Hash(pObjectKey);

  if(hash == -1)
    {
      PyErr_SetString(PyExc_RuntimeError,
                      "key object is not hashable");

      return nullptr;
    }

  std::vector<OpenStatistic::Any> anys{};

  try
    {
      PyObject_to_any_container(pObjectValues,anys);
    }
  catch(std::exception & exp)
    {
      PyErr_SetString(PyExc_RuntimeError,
                      exp.what());
      return nullptr;
    }

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      pSelf->pTable_->setRow(hash,anys);
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

PyDoc_STRVAR(ExtTable_clear_doc,
             "clear(self)\n\n"
             "Clears an OpenStatistic table.\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             "Example:\n"
             "    foo_table.clear()\n"
             "\n");
static PyObject *
ExtTable_clear(PyObject * self, PyObject *args)
{
  ExtTable * pSelf{reinterpret_cast<ExtTable *>(self)};

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      pSelf->pTable_->clear();
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


PyDoc_STRVAR(ExtTable_set_cell_doc,
             "set_cell(self,args)\n\n"
             "Sets an individual OpenStatistic table row cell.\n"
             "\n"
             "Args:\n"
             "    key (obj): A hashable key object.\n"
             "    cell (int): Index of cell to set (0 based).\n"
             "    value (int|bool|double|str): Row cell value.\n\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             "Example:\n"
             "    foo_table.set_cell(6,1,'INACTIVE')\n"
             "\n");
static PyObject *
ExtTable_set_cell(PyObject * self, PyObject *args)
{
  ExtTable * pSelf{reinterpret_cast<ExtTable *>(self)};

  std::uint64_t u64Column{};

  PyObject * pObjectKey{};
  PyObject * pObjectValue{};

  if(!PyArg_ParseTuple(args,
                       "OKO",
                       &pObjectKey,
                       &u64Column,
                       &pObjectValue))
    {
      return nullptr;
    }

  auto hash = PyObject_Hash(pObjectKey);

  if(hash == -1)
    {
      PyErr_SetString(PyExc_RuntimeError,
                      "key object is not hashable");

      return nullptr;
    }

  OpenStatistic::Any any{0L};

  try
    {
      any = PyObject_to_any(pObjectValue);
    }
  catch(std::exception & exp)
    {
      PyErr_SetString(PyExc_RuntimeError,
                      exp.what());
      return nullptr;
    }

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      pSelf->pTable_->setCell(hash,u64Column,any);
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



static PyObject *
ExtTable_str(PyObject * self)
{
  return nullptr;
};

static PyMethodDef ExtTable_methods[] =
  {
    {
      "set_cell",
      reinterpret_cast<PyCFunction>(ExtTable_set_cell),
      METH_VARARGS,
      ExtTable_set_cell_doc,
    },

    {
      "add_row",
      reinterpret_cast<PyCFunction>(ExtTable_add_row),
      METH_VARARGS,
      ExtTable_add_row_doc,
    },

    {
      "set_row",
      reinterpret_cast<PyCFunction>(ExtTable_set_row),
      METH_VARARGS,
      ExtTable_set_row_doc,
    },


    {
      "delete_row",
      reinterpret_cast<PyCFunction>(ExtTable_delete_row),
      METH_VARARGS,
      ExtTable_delete_row_doc,
    },

    {
      "clear",
      reinterpret_cast<PyCFunction>(ExtTable_clear),
      METH_NOARGS,
      ExtTable_clear_doc,
    },

    {nullptr, nullptr, 0, nullptr}
  };

PyDoc_STRVAR(ExtTable_type_doc,
             "OpenStatistic Table extension\n");
PyTypeObject ExtTableType =
  {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Double",
    sizeof(ExtTable), // tp_basicsize
    0, // tp_itemsize
    reinterpret_cast<destructor>(ExtTable_dealloc), // tp_dealloc
    0, // tp_print
    0, // tp_getattr
    0, // tp_setattr
    0, // tp_compare
    ExtTable_repr, // tp_repr
    0, // tp_as_number
    0, // tp_as_sequence
    0, // tp_as_mapping
    0, // tp_hash
    0, // tp_call
    ExtTable_str, // tp_str
    0, // tp_getattro
    0, // tp_setattro
    0, // tp_as_buffer
    Py_TPFLAGS_DEFAULT, // tp_flags
    ExtTable_type_doc, // tp_doc
    0, // tp_traverse
    0, // tp_clear
    0, // tp_richcompare
    0, // tp_weaklistoffset
    0, // tp_iter
    0, // tp_iternext
    ExtTable_methods, // tp_methods
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
ExtTable_FromTable(const std::string & sName,
                   OpenStatistic::Table<Py_hash_t> * pTable,
                   PyObject * pClearCallableObject)
{
  ExtTable* pExtTable =
    reinterpret_cast<ExtTable *>(ExtTableType.tp_alloc(&ExtTableType, 0));

  new (pExtTable) ExtTable{sName,pTable,pClearCallableObject};

  return reinterpret_cast<PyObject *>(pExtTable);
}
