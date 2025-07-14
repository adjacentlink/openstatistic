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

#include "extservice.h"
#include "extsignedinteger.h"
#include "extunsignedinteger.h"
#include "extdouble.h"
#include "extboolean.h"
#include "extstring.h"
#include "exttable.h"
#include "pyobjectutils.h"

ExtService::ExtService(OpenStatistic::Service * pService):
  pService_{pService}
{}

enum StatisticType
  {
    TYPE_SIGNED_INTEGER = 1,
    TYPE_UNSIGNED_INTEGER = 2,
    TYPE_DOUBLE = 3,
    TYPE_STRING = 4,
    TYPE_BOOLEAN = 5
  };

static void
ExtService_dealloc(ExtService * self)
{
  self->~ExtService();

  reinterpret_cast<PyObject*>(self)->ob_type->tp_free(reinterpret_cast<PyObject*>(self));
}


static PyObject *
ExtService_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  ExtService * pSelf{};

  pSelf = reinterpret_cast<ExtService *>(type->tp_alloc(type, 0));

  new (pSelf) ExtService{OpenStatistic::Service::instance()};

  return reinterpret_cast<PyObject *>(pSelf);
}


PyDoc_STRVAR(ExtService_start_doc,
             "start(self,args,kwargs)\n\n"
             "Starts the OpenStatistic service.\n"
             "\n"
             "Args:\n"
             "    listen_endpoint (str): Listen endpoint of the service in the\n"
             "        form: IPv4:PORT or [IPv6]:Port.\n\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             "Example::\n"
             "    import ostatistic.lib as ostatlib\n"
             "\n"
             "    service = ostatlib.Service()\n"
             "\n"
             "    service.start('10.99.0.1:8008')\n"
             "\n");
static PyObject *
ExtService_start(PyObject * self, PyObject *args)
{
  ExtService * pSelf{reinterpret_cast<ExtService *>(self)};

  const char * pzListenEndpoint{};

  if(!PyArg_ParseTuple(args,
                       "s",
                       &pzListenEndpoint))
    {
      return NULL;
    }

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      pSelf->pService_->start(pzListenEndpoint);
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


PyDoc_STRVAR(ExtService_stop_doc,
             "stop(self)\n\n"
             "Stops the OpenStatisticService service.\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             );

static PyObject * ExtService_stop(PyObject * self)
{
  ExtService * pSelf{reinterpret_cast<ExtService *>(self)};

  std::string sError{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      pSelf->pService_->stop();
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

PyDoc_STRVAR(ExtService_register_statistic_doc,
             "register_statistic(self,args,kwargs)\n\n"
             "Registers an OpenStatistic statistic.\n"
             "\n"
             "Args:\n"
             "    type (int): Type of statistic oone of: STAT_UINTEGER,\n"
             "        STAT_SINTEGER, STAT_DOUBLE, STATE_BOOLEAN,\n"
             "        STAT_STRING.\n\n"
             "    name (str): Name of the statistic. Valid characters are\n"
             "        [A-Za-z0-9.].\n\n"
             "    description (str): Description of the statistic.\n\n"
             "\n"
             "Keyword Args:\n"
             "    clearable (bool): Flag to enable clearing the statistic.\n"
             "        Default: False.\n\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             "Example::\n"
             "    import ostatistic.lib as ostatlib\n"
             "\n"
             "    statistic_service = ostatlib.Service()\n"
             "\n"
             "    foo_uint = \\\n"
             "    stat_service.register_statistic(stat_service.STAT_UINTEGER,\n"
             "                                    'foobar',\n"
             "                                    'Counts all foo',\n"
             "                                    clearable=True)\n"
             "\n");
static PyObject *
ExtService_register_statistic(PyObject * self, PyObject *args, PyObject *kwargs)
{
  ExtService * pSelf{reinterpret_cast<ExtService *>(self)};

  const char * pzName{};
  const char * pzDescription{};
  unsigned int uType{};
  unsigned int uClearable{};

  const char * kwlist[] =
    {
      "type",
      "name",
      "description",
      "clearable",
      nullptr
    };

  if(!PyArg_ParseTupleAndKeywords(args,
                                  kwargs,
                                  "Iss|I",
                                  const_cast<char **>(kwlist),
                                  &uType,
                                  &pzName,
                                  &pzDescription,
                                  &uClearable))
    {
      return NULL;
    }

  std::string sError{};

  OpenStatistic::StatisticNumeric<std::int64_t> * pi64Statistic{};
  OpenStatistic::StatisticNumeric<std::uint64_t> * pu64Statistic{};
  OpenStatistic::StatisticNumeric<double> * pdStatistic{};
  OpenStatistic::StatisticNumeric<bool> * pbStatistic{};
  OpenStatistic::StatisticNonNumeric<std::string> * psStatistic{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      switch(uType)
        {
        case TYPE_SIGNED_INTEGER:
          pi64Statistic = pSelf->pService_->registrar().
            registerNumeric<std::int64_t>(pzName,
                                          uClearable ?
                                          OpenStatistic::StatisticProperties::CLEARABLE :
                                          OpenStatistic::StatisticProperties::NONE,
                                          pzDescription);
          break;

        case TYPE_UNSIGNED_INTEGER:
          pu64Statistic = pSelf->pService_->registrar().
            registerNumeric<std::uint64_t>(pzName,
                                           uClearable ?
                                           OpenStatistic::StatisticProperties::CLEARABLE :
                                           OpenStatistic::StatisticProperties::NONE,
                                           pzDescription);
          break;

        case TYPE_DOUBLE:
          pdStatistic = pSelf->pService_->registrar().
            registerNumeric<double>(pzName,
                                    uClearable ?
                                    OpenStatistic::StatisticProperties::CLEARABLE :
                                    OpenStatistic::StatisticProperties::NONE,
                                    pzDescription);

          break;

        case TYPE_BOOLEAN:
          pbStatistic = pSelf->pService_->registrar().
            registerNumeric<bool>(pzName,
                                  uClearable ?
                                  OpenStatistic::StatisticProperties::CLEARABLE :
                                  OpenStatistic::StatisticProperties::NONE,
                                  pzDescription);

          break;

        case TYPE_STRING:
          psStatistic = pSelf->pService_->registrar().
            registerNonNumeric<std::string>(pzName,
                                            uClearable ?
                                            OpenStatistic::StatisticProperties::CLEARABLE :
                                            OpenStatistic::StatisticProperties::NONE,
                                            pzDescription);

          break;

        default:
          sError = "unknown statsitic type";
          break;
        }
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

  switch(uType)
    {
    case TYPE_SIGNED_INTEGER:
      return ExtSignedInteger_FromStatisticNumeric(pzName,
                                                   pi64Statistic);

    case TYPE_UNSIGNED_INTEGER:
      return ExtUnsignedInteger_FromStatisticNumeric(pzName,
                                                     pu64Statistic);

    case TYPE_DOUBLE:
      return ExtDouble_FromStatisticNumeric(pzName,
                                            pdStatistic);

    case TYPE_BOOLEAN:
      return ExtBoolean_FromStatisticNumeric(pzName,
                                             pbStatistic);

    case TYPE_STRING:
      return ExtString_FromStatisticNumeric(pzName,
                                            psStatistic);

    default:
      PyErr_SetString(PyExc_RuntimeError,
                      "unknown statsitic type");

      return nullptr;
    }
}


PyDoc_STRVAR(ExtService_register_table_doc,
             "register_statistic(self,args,kwargs)\n\n"
             "Registers an OpenStatistic table.\n"
             "\n"
             "Args:\n"
             "    name (str): Name of the table. Valid characters are\n"
             "        [A-Za-z0-9.].\n\n"
             "    description (str): Description of the statistic.\n\n"
             "    labels (tuple(str)): Names of each of the table columns.\n\n"
             "\n"
             "Keyword Args:\n"
             "    clearable (bool): Flag to enable clearing the statistic.\n"
             "        Default: False.\n\n"
             "    clear_func (callable): Function to invoke of clear. Takes\n"
             "        a single argument, table object to clear. Useful when\n"
             "        data represented in table must be modified on clear.\n\n"
             "\n"
             "Raises:\n"
             "    RuntimeError: If an OpenStatistic error is encountered.\n\n"
             "\n"
             "Example:\n"
             "def clear(table):\n"
             "    # perfrom any work on data that was used to populate table\n"
             "    table.clear()\n"
             "\n"
             "foo_table = \\\n"
             "stat_service.register_table('foo.table',\n"
             "                            'My foo table.'\n"
             "                            ('Column0','Column1'),\n"
             "                            clearable = True,\n"
             "                            clear_func=clear)\n"
             "\n");
static PyObject *
ExtService_register_table(PyObject * self, PyObject *args, PyObject *kwargs)
{
  ExtService * pSelf{reinterpret_cast<ExtService *>(self)};

  const char * pzName{};
  const char * pzDescription{};
  unsigned int uClearable{};
  PyObject * pObjectLabels{};
  PyObject * pClearCallable{};

  const char * kwlist[] =
    {
      "name",
      "description",
      "labels",
      "clearable",
      "clear_func",
      nullptr
    };

  if(!PyArg_ParseTupleAndKeywords(args,
                                  kwargs,
                                  "ssO|IO",
                                  const_cast<char **>(kwlist),
                                  &pzName,
                                  &pzDescription,
                                  &pObjectLabels,
                                  &uClearable,
                                  &pClearCallable))
    {
      return nullptr;
    }

  OpenStatistic::TableLabels tableLabels{};

  try
    {
      PyObject_to_string_container(pObjectLabels,
                                   tableLabels);
    }
  catch(std::invalid_argument & exp)
    {
      PyErr_SetString(PyExc_RuntimeError,
                      "'labels' must be list or tuple of strings");
      return nullptr;
    }

  std::string sError{};

  OpenStatistic::Table<Py_hash_t> * pTable{};

  Py_BEGIN_ALLOW_THREADS;

  try
    {
      if(uClearable)
        {
          if(pClearCallable)
            {
              pTable = pSelf->pService_->registrar().
                registerTable<Py_hash_t>(pzName,
                                         tableLabels,
                                         [pSelf](OpenStatistic::TablePublisher * pTablePublisher)
                                         {
                                           const auto iter = pSelf->tableMap_.find(pTablePublisher);

                                           if(iter != pSelf->tableMap_.end())
                                             {
                                               PyGILState_STATE state;

                                               state = PyGILState_Ensure();

                                               PyObject * pReturn =
                                                 PyObject_CallFunctionObjArgs(iter->second->pClearCallableObject_,
                                                                              iter->second,
                                                                              nullptr);

                                               if(pReturn)
                                                 {
                                                   Py_DECREF(pReturn);
                                                 }
                                               else
                                                 {
                                                   PyErr_PrintEx(1);
                                                 }

                                               PyGILState_Release(state);
                                             }
                                         },
                                         pzDescription);
            }
          else
            {
              pTable = pSelf->pService_->registrar().
                registerTable<Py_hash_t>(pzName,
                                         tableLabels,
                                         OpenStatistic::StatisticProperties::CLEARABLE,
                                         pzDescription);
            }
        }
      else
        {
          pTable = pSelf->pService_->registrar().
            registerTable<Py_hash_t>(pzName,
                                     tableLabels,
                                     OpenStatistic::StatisticProperties::NONE,
                                     pzDescription);
        }
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

  Py_INCREF(pClearCallable);

  PyObject * pTableObject = ExtTable_FromTable(pzName,pTable,pClearCallable);

  pSelf->tableMap_.emplace(pTable,reinterpret_cast<ExtTable *>(pTableObject));

  return pTableObject;
}


static PyObject * ExtService_dict_i()
{
  PyObject * pDict = PyDict_New();

  PyDict_SetItemString(pDict,
                       "STAT_SINTEGER",
                       PyLong_FromLong(TYPE_SIGNED_INTEGER));

  PyDict_SetItemString(pDict,
                       "STAT_UINTEGER",
                       PyLong_FromLong(TYPE_UNSIGNED_INTEGER));

  PyDict_SetItemString(pDict,
                       "STAT_DOUBLE",
                       PyLong_FromLong(TYPE_DOUBLE));

  PyDict_SetItemString(pDict,
                       "STAT_STRING",
                       PyLong_FromLong(TYPE_STRING));

  PyDict_SetItemString(pDict,
                       "STAT_BOOLEAN",
                       PyLong_FromLong(TYPE_BOOLEAN));

  return pDict;
}

static PyMethodDef ExtService_methods[] =
  {
    {
      "start",
      reinterpret_cast<PyCFunction>(ExtService_start),
      METH_VARARGS,
      ExtService_start_doc,
    },

    {
      "stop",
      reinterpret_cast<PyCFunction>(ExtService_stop),
      METH_NOARGS,
      ExtService_stop_doc,
    },

    {
      "register_statistic",
      reinterpret_cast<PyCFunction>(ExtService_register_statistic),
      METH_VARARGS | METH_KEYWORDS,
      ExtService_register_statistic_doc,
    },

    {
      "register_table",
      reinterpret_cast<PyCFunction>(ExtService_register_table),
      METH_VARARGS | METH_KEYWORDS,
      ExtService_register_table_doc,
    },

    {nullptr, nullptr, 0, nullptr}
  };


PyDoc_STRVAR(ExtService_type_doc,
             "OpenStatistic extension\n\n"
             "OpenStatistic service instance.");

PyTypeObject ExtServiceType =
  {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Service",
    sizeof(ExtService), // tp_basicsize
    0, // tp_itemsize
    reinterpret_cast<destructor>(ExtService_dealloc), // tp_dealloc
    0, // tp_print
    0, // tp_getattr
    0, // tp_setattr
    0, // tp_compare
    0, // tp_repr
    0, // tp_as_number
    0, // tp_as_sequence
    0, // tp_as_mapping
    0, // tp_hash
    0, // tp_call
    0, // tp_str
    0, // tp_getattro
    0, // tp_setattro
    0, // tp_as_buffer
    Py_TPFLAGS_DEFAULT, // tp_flags
    ExtService_type_doc, // tp_doc
    0, // tp_traverse
    0, // tp_clear
    0, // tp_richcompare
    0, // tp_weaklistoffset
    0, // tp_iter
    0, // tp_iternext
    ExtService_methods, // tp_methods
    0, // tp_members
    0, // tp_getset
    0, // tp_base
    ExtService_dict_i(), // tp_dict
    0, // tp_descr_get
    0, // tp_descr_set
    0, // tp_dictoffset
    0, // tp_init
    0, // tp_alloc
    ExtService_new, // tp_new
  };
