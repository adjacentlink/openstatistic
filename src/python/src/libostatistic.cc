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

#include <Python.h>

#include "extservice.h"
#include "extsignedinteger.h"
#include "extunsignedinteger.h"
#include "extdouble.h"
#include "extboolean.h"
#include "extstring.h"
#include "exttable.h"

PyDoc_STRVAR(ostatistic_module_doc,
             "Extension for OpenStatistic.\n\n"
             "\n"
             "OpenStatistic is a minimally intrusive library for instrumenting\n"
             "C++ applications with measurement capabilities and realtime data\n"
             "extraction."
             );

static PyModuleDef ostatistic_lib_module =
  {
    PyModuleDef_HEAD_INIT,
    "ostatistic.lib",
    ostatistic_module_doc,
    -1,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
  };

PyMODINIT_FUNC
PyInit_lib(void)
{
  PyObject* m;

  if(PyType_Ready(&ExtServiceType) < 0)
    {
      return nullptr;
    }

  if(PyType_Ready(&ExtUnsignedIntegerType) < 0)
    {
      return nullptr;
    }

  if(PyType_Ready(&ExtSignedIntegerType) < 0)
    {
      return nullptr;
    }

  if(PyType_Ready(&ExtDoubleType) < 0)
    {
      return nullptr;
    }

  if(PyType_Ready(&ExtBooleanType) < 0)
    {
      return nullptr;
    }

  if(PyType_Ready(&ExtStringType) < 0)
    {
      return nullptr;
    }

  if(PyType_Ready(&ExtTableType) < 0)
    {
      return nullptr;
    }

  m = PyModule_Create(&ostatistic_lib_module);

  if (m == nullptr)
    {
      return nullptr;
    }

  Py_INCREF(&ExtServiceType);

  Py_INCREF(&ExtUnsignedIntegerType);

  Py_INCREF(&ExtSignedIntegerType);

  Py_INCREF(&ExtDoubleType);

  Py_INCREF(&ExtBooleanType);

  Py_INCREF(&ExtStringType);

  Py_INCREF(&ExtTableType);

  PyModule_AddObject(m,"Service",(PyObject *)&ExtServiceType);

  PyModule_AddObject(m,"UnsignedInteger",(PyObject *)&ExtUnsignedIntegerType);

  PyModule_AddObject(m,"SignedInteger",(PyObject *)&ExtSignedIntegerType);

  PyModule_AddObject(m,"Double",(PyObject *)&ExtDoubleType);

  PyModule_AddObject(m,"Boolean",(PyObject *)&ExtBooleanType);

  PyModule_AddObject(m,"String",(PyObject *)&ExtStringType);

  PyModule_AddObject(m,"Table",(PyObject *)&ExtTableType);

  return m;
}
