/*
 * Copyright (c) 2013,2015-2016 - Adjacent Link LLC, Bridgewater,
 * New Jersey
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of Adjacent Link LLC nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
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

#ifndef OPENSTATISTIC_ANYUTILS_HEADER_
#define OPENSTATISTIC_ANYUTILS_HEADER_

#include "openstatisticapi.pb.h"
#include "ostatistic/any.h"

namespace OpenStatistic
{
  inline
  Any::Type toAnyType(OpenStatisticAPI::Any::AnyType type)
  {
    switch(type)
      {
      case OpenStatisticAPI::Any::TYPE_ANY_INT64:
        return Any::Type::TYPE_INT64;

      case OpenStatisticAPI::Any::TYPE_ANY_UINT64:
        return Any::Type::TYPE_UINT64;

      case OpenStatisticAPI::Any::TYPE_ANY_DOUBLE:
        return Any::Type::TYPE_DOUBLE;

      case OpenStatisticAPI::Any::TYPE_ANY_STRING:
        return Any::Type::TYPE_STRING;

      case OpenStatisticAPI::Any::TYPE_ANY_BOOLEAN:
        return Any::Type::TYPE_BOOL;
      }

    throw Exception("unknown type");
  }

  inline
  Any toAny(OpenStatisticAPI::Any any)
  {
    switch(any.type())
      {
      case OpenStatisticAPI::Any::TYPE_ANY_INT64:
        return Any{any.i64value()};

      case OpenStatisticAPI::Any::TYPE_ANY_UINT64:
        return Any{any.u64value()};

      case OpenStatisticAPI::Any::TYPE_ANY_DOUBLE:
        return Any{any.dvalue()};

      case OpenStatisticAPI::Any::TYPE_ANY_STRING:
        return Any{any.svalue()};

      case OpenStatisticAPI::Any::TYPE_ANY_BOOLEAN:
        return Any{any.bvalue()};
      }

    throw Exception("unknown type");
  }

  inline
  void convertToAny(OpenStatisticAPI::Any * pAny,
                    const Any & any)
  {
    switch(any.getType())
      {
      case Any::Type::TYPE_INT64:
        pAny->set_type(OpenStatisticAPI::Any::TYPE_ANY_INT64);
        pAny->set_i64value(any.asINT64());
        break;

      case Any::Type::TYPE_UINT64:
        pAny->set_type(OpenStatisticAPI::Any::TYPE_ANY_UINT64);
        pAny->set_u64value(any.asUINT64());
        break;

      case Any::Type::TYPE_DOUBLE:
        pAny->set_type(OpenStatisticAPI::Any::TYPE_ANY_DOUBLE);
        pAny->set_dvalue(any.asDouble());
        break;

      case Any::Type::TYPE_BOOL:
        pAny->set_type(OpenStatisticAPI::Any::TYPE_ANY_BOOLEAN);
        pAny->set_bvalue(any.asBool());
        break;

      case Any::Type::TYPE_STRING:
        pAny->set_type(OpenStatisticAPI::Any::TYPE_ANY_STRING);
        pAny->set_svalue(any.asString());
        break;

      default:
        throw Exception("unknown type");
      }
  }
}




#endif // OPENSTATISTIC_ANYUTILS_HEADER_
