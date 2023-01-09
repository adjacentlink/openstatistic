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

#include "ostatistic/any.h"
#include "parameterconvert.h"

OpenStatistic::Any::Any(std::int64_t i64Value):
  type_{Type::TYPE_INT64},
  i64Value_{i64Value}{}

OpenStatistic::Any::Any(std::uint64_t u64Value):
  type_{Type::TYPE_UINT64},
  u64Value_{u64Value}{}

OpenStatistic::Any::Any(double dValue):
  type_{Type::TYPE_DOUBLE},
  dValue_{dValue}{}

OpenStatistic::Any::Any(const bool & bValue):
  type_{Type::TYPE_BOOL},
  u64Value_(bValue){}

OpenStatistic::Any::Any(const char * pzValue):
  type_{Type::TYPE_STRING},
  sValue_{pzValue}{}

OpenStatistic::Any::Any(const std::string & sValue):
  type_{Type::TYPE_STRING},
  sValue_{sValue}{}

OpenStatistic::Any::Any(const Any& rhs)
{
  type_ = rhs.type_;

  if(type_ == Type::TYPE_STRING)
    {
      new (&sValue_) std::string(rhs.sValue_);
    }
  else
    {
      // copy state  as binary represenation
      memcpy(reinterpret_cast<char *>(this),
             reinterpret_cast<const char *>(&rhs),
             sizeof(Any));
    }
}

OpenStatistic::Any & OpenStatistic::Any::operator=(const Any & rhs)
{
  // if std::string or INETAddr cleanup
  this->~Any();

  type_ = rhs.type_;

  // if std::string or INETAddr use placement new
  // to create object
  if(type_ == Type::TYPE_STRING)
    {
      new (&sValue_) std::string(rhs.sValue_);
    }
  else
    {
      // copy state  as binary represenation
       memcpy(reinterpret_cast<char *>(this),
             reinterpret_cast<const char *>(&rhs),
             sizeof(Any));
    }

  return *this;
}

OpenStatistic::Any::~Any()
{
  // if std::string or INETAddr cleanup
  if(type_ == Type::TYPE_STRING)
    {
      sValue_.~basic_string<char>();
    }
}

std::int64_t OpenStatistic::Any::asINT64() const
{
  if(type_ == Type::TYPE_INT64)
    {
      return i64Value_;
    }

  throw Exception("Any not INT64");
}

std::uint64_t OpenStatistic::Any::asUINT64() const
{
  if(type_ == Type::TYPE_UINT64)
    {
      return u64Value_;
    }

  throw Exception("Any not UINT64");
}

double OpenStatistic::Any::asDouble() const
{
  if(type_ == Type::TYPE_DOUBLE)
    {
      return dValue_;
    }

  throw Exception("Any not Double");
}

bool OpenStatistic::Any::asBool() const
{
  if(type_ == Type::TYPE_BOOL)
    {
      return u64Value_;
    }

  throw Exception("Any Not Bool");
}


std::string OpenStatistic::Any::asString() const
{
  if(type_ == Type::TYPE_STRING)
    {
      return sValue_;
    }

  throw Exception("Ant not String");
}

OpenStatistic::Any::Type OpenStatistic::Any::getType() const
{
  return type_;
}

std::ostream & operator<<(std::ostream &out, const OpenStatistic::Any & any)
{
  out<<any.toString();
  return out;
}

std::string OpenStatistic::Any::toString() const
{
  switch(getType())
    {
    case OpenStatistic::Any::Type::TYPE_INT64:
      return std::to_string(asINT64());

    case OpenStatistic::Any::Type::TYPE_UINT64:
      return std::to_string(asUINT64());

    case OpenStatistic::Any::Type::TYPE_DOUBLE:
      return std::to_string(asDouble());

    case OpenStatistic::Any::Type::TYPE_BOOL:
      return std::to_string(asBool());

    case OpenStatistic::Any::Type::TYPE_STRING:
      return asString();

    default:
      break;
    }

  return {};
}

OpenStatistic::Any OpenStatistic::Any::create(std::string sValue, Type type)
{
  switch(type)
    {
    case Any::Type::TYPE_INT64:
      return Any(ParameterConvert(sValue).toINT64());

    case Any::Type::TYPE_UINT64:
      return Any(ParameterConvert(sValue).toUINT64());

    case Any::Type::TYPE_DOUBLE:
      return Any(ParameterConvert(sValue).toDouble());

    case Any::Type::TYPE_BOOL:
      return Any(ParameterConvert(sValue).toBool());

    case Any::Type::TYPE_STRING:
      return Any(sValue.c_str());

    default:
      break;
    }

  throw Exception("Any cannot be created from type");
}

bool OpenStatistic::Any::operator<=(const OpenStatistic::Any & rhs) const
{
  if(type_ == rhs.type_)
    {
      switch(type_)
        {
        case Any::Type::TYPE_INT64:
          return i64Value_ <= rhs.i64Value_;

        case Any::Type::TYPE_UINT64:
        case Any::Type::TYPE_BOOL:
          return u64Value_ <= rhs.u64Value_;

        case Any::Type::TYPE_DOUBLE:
          return dValue_ <= rhs.dValue_;

        case Any::Type::TYPE_STRING:
          return sValue_ <= rhs.sValue_;

        default:
          break;
        }
    }

  throw Exception("Type mismatch");
}

bool OpenStatistic::Any::operator>=(const OpenStatistic::Any & rhs) const
{
  if(type_ == rhs.type_)
    {
      switch(type_)
        {
        case Any::Type::TYPE_INT64:
          return i64Value_ >=rhs.i64Value_;

        case Any::Type::TYPE_UINT64:
        case Any::Type::TYPE_BOOL:
          return u64Value_ >= rhs.u64Value_;

        case Any::Type::TYPE_DOUBLE:
          return dValue_ >= rhs.dValue_;

        case Any::Type::TYPE_STRING:
          return sValue_ >= rhs.sValue_;

        default:
          break;
        }
    }

  throw Exception("Ant type mismatch");
}

bool OpenStatistic::Any::operator<(const OpenStatistic::Any & rhs) const
{
  if(type_ == rhs.type_)
    {
      switch(type_)
        {
        case Any::Type::TYPE_INT64:
          return i64Value_ < rhs.i64Value_;

        case Any::Type::TYPE_UINT64:
        case Any::Type::TYPE_BOOL:
          return u64Value_ < rhs.u64Value_;

        case Any::Type::TYPE_DOUBLE:
          return dValue_ < rhs.dValue_;

        case Any::Type::TYPE_STRING:
          return sValue_ < rhs.sValue_;

        default:
          break;
        }
    }

  throw Exception("Type mismatch");
}

bool OpenStatistic::Any::operator>(const OpenStatistic::Any & rhs) const
{
  if(type_ == rhs.type_)
    {
      switch(type_)
        {
        case Any::Type::TYPE_INT64:
          return i64Value_ > rhs.i64Value_;

        case Any::Type::TYPE_UINT64:
        case Any::Type::TYPE_BOOL:
          return u64Value_ > rhs.u64Value_;

        case Any::Type::TYPE_DOUBLE:
          return dValue_ > rhs.dValue_;

        case Any::Type::TYPE_STRING:
          return sValue_ > rhs.sValue_;

        default:
          break;
        }
    }

  throw Exception("Type mismatch");
}

std::string OpenStatistic::anyTypeAsString(const Any::Type & type)
{
  switch(type)
    {
    case Any::Type::TYPE_UINT64:
      return "uint64";

    case Any::Type::TYPE_INT64:
      return "int64";

    case Any::Type::TYPE_BOOL:
      return "bool";

    case Any::Type::TYPE_DOUBLE:
      return "double";

    case Any::Type::TYPE_STRING:
      return "string";

    default:
      break;
    }

  throw Exception("Invalid type");
}

namespace OpenStatistic
{
  template <>
  Any::Type AnyConvertableType<std::uint64_t>::type()
  {
    return Any::Type::TYPE_UINT64;
  }

  template <>
  Any::Type AnyConvertableType<std::int64_t>::type()
  {
    return Any::Type::TYPE_INT64;
  }

  template <>
  Any::Type AnyConvertableType<bool>::type()
  {
    return Any::Type::TYPE_BOOL;
  }

  template <>
  Any::Type AnyConvertableType<double>::type()
  {
    return Any::Type::TYPE_DOUBLE;
  }

  template <>
  Any::Type AnyConvertableType<const char *>::type()
  {
    return Any::Type::TYPE_STRING;
  }

  template <>
  Any::Type AnyConvertableType<std::string>::type()
  {
    return Any::Type::TYPE_STRING;
  }
}
