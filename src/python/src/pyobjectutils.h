/*
 * Copyright (c) 2018,2024 - Adjacent Link LLC, Bridgewater, New Jersey
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

#ifndef PYOBJECTUTILS_HEADER_
#define PYOBJECTUTILS_HEADER_

#include "ostatistic/any.h"

#include <limits>
#include <stdexcept>

template <typename UnaryFunction>
void PyObject_for_each(PyObject * pObject,
                       UnaryFunction f)
{
  Py_ssize_t size{};
  PyObject * (*GetItem)(PyObject *, Py_ssize_t){};

  if(PyTuple_Check(pObject))
    {
      size = PyTuple_Size(pObject);
      GetItem = &PyTuple_GetItem;
    }
  else if(PyList_Check(pObject))
    {
      size = PyList_Size(pObject);
      GetItem = &PyList_GetItem;
    }
  else
    {
      throw std::invalid_argument{"not list or tuple"};
    }

  for(Py_ssize_t i = 0; i < size; ++i)
    {
      f(GetItem(pObject,i));
    }
}

inline
OpenStatistic::Any
PyObject_to_any(PyObject * pObject)
{
  OpenStatistic::Any any{"na"};

  if(PyBool_Check(pObject))
    {
      any = OpenStatistic::Any{static_cast<bool>(pObject == Py_True)};
    }
  else if(PyLong_Check(pObject))
    {
      auto value = PyLong_AsUnsignedLongLong(pObject);

      if(PyErr_Occurred())
        {
          PyErr_Clear();

          auto value = PyLong_AsLongLong(pObject);

          if(PyErr_Occurred())
            {
              throw std::out_of_range{"integer value out of range"};
            }
          else
            {
              any = OpenStatistic::Any{static_cast<std::int64_t>(value)};
            }
        }
      else
        {
          any = OpenStatistic::Any{static_cast<std::uint64_t>(value)};
        }
    }
  else if(PyFloat_Check(pObject))
    {
      double dValue = PyFloat_AsDouble(pObject);

      if(PyErr_Occurred())
        {
          throw std::out_of_range{"float value out of range"};
        }

      any = OpenStatistic::Any{dValue};
    }
  else if(PyUnicode_Check(pObject))
    {
      any = OpenStatistic::Any{PyUnicode_AsUTF8(pObject)};
    }
  else
    {
      throw std::invalid_argument{"argument not any convertible"};
    }

  return any;
}

inline
std::string
PyObject_to_string(PyObject * pObject)
{
  std::string sValue{};

  if(PyUnicode_Check(pObject))
    {
      sValue = PyUnicode_AsUTF8(pObject);
    }
  else
    {
      throw std::invalid_argument{"argument not any convertible"};
    }

  return sValue;
}

template <typename T>
void PyObject_to_any_container(PyObject * pObject,
                               T & container)
{
  auto tmp = T{};

  PyObject_for_each(pObject,
                    [&tmp](PyObject * p)
                    {
                      tmp.push_back(PyObject_to_any(p));
                    });

  container = std::move(tmp);
}

template <typename T>
void PyObject_to_string_container(PyObject * pObject,
                                  T & container)
{
  auto tmp = T{};

  PyObject_for_each(pObject,
                    [&tmp](PyObject * p)
                    {
                      tmp.push_back(PyObject_to_string(p));
                    });

  container = std::move(tmp);
}

#endif // PYOBJECTUTILS_HEADER_
