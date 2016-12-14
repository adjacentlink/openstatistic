/*
 * Copyright (c) 2013-2016 - Adjacent Link LLC, Bridgewater, New Jersey
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

#ifndef OPENSTATISTIC_ANY_HEADER_
#define OPENSTATISTIC_ANY_HEADER_

#include "ostatistic/exception.h"

#include <cstdint>

namespace OpenStatistic
{
  /**
   * @class Any
   *
   * @brief The Any class can contain an instance of one of
   * any type in its support type set.
   */
  class Any
  {
  public:
    enum class Type
    {
      TYPE_INT64,
        TYPE_UINT64,
        TYPE_DOUBLE,
        TYPE_BOOL,
        TYPE_STRING,
        };

    /**
     * Creates an Any instance from a signed 64-bit integer
     */
    explicit Any(std::int64_t i64Value);

    /**
     * Creates an Any instance from an unsigned 64-bit integer
     */
    explicit Any(std::uint64_t u64Value);

    /**
     * Creates an Any instance from a double
     */
    explicit Any(double dValue);


    /**
     * Creates an Any instance from a NULL terminated string
     */
    explicit Any(const char * pzValue);

    /**
     * Creates an Any instance from a std::string
     */
    explicit Any(const std::string & sValue);

    /**
     * Creates an Any instance from a boolean
     */
    explicit Any(const bool & bValue);

    /**
     * Creates an Any instance from another instance
     */
    Any(const Any & rhs);

    /**
     * Creates an Any instance from another instance
     */
    Any & operator=(const Any & rhs);

    /**
     * Destroys an instance
     */
    ~Any();

    /**
     * Gets the signed 64-bit integer Any value
     *
     * @return value
     *
     * @throw AnyException if value is a different type
     */
    std::int64_t asINT64() const;

    /**
     * Gets the unsigned 64-bit integer Any value
     *
     * @return value
     *
     * @throw AnyException if value is a different type
     */
    std::uint64_t asUINT64() const;


    /**
     * Gets the double Any value
     *
     * @return value
     *
     * @throw AnyException if value is a different type
     */
    double asDouble() const;


    /**
     * Gets the boolean Any value
     *
     * @return value
     *
     * @throw AnyException if value is a different type
     */
    bool asBool() const;

    /**
     * Gets the string Any value
     *
     * @return value
     *
     * @throw AnyException if value is a different type
     */
    std::string asString() const;

    /**
     * Gets the Any type
     *
     * @return value
     */
    Type getType() const;

    /**
     * Less than comparison
     *
     * @param rhs INstance to compare

     * @return @a true if less than
     */
    bool operator<(const OpenStatistic::Any & rhs) const;

    /**
     * Greater than comparison
     *
     * @param rhs INstance to compare

     * @return @a true if greater than
     */
    bool operator>(const OpenStatistic::Any & rhs) const;

    /**
     * Less than or equal comparison
     *
     * @param rhs INstance to compare

     * @return @a true if less than or equal
     */
    bool operator<=(const OpenStatistic::Any & rhs) const;

    /**
     * Greater than or equal comparison
     *
     * @param rhs INstance to compare

     * @return @a true if greater than or equal
     */
    bool operator>=(const OpenStatistic::Any & rhs) const;

    /**
     * Gets the stringified Any value
     *
     * @return value as string
     */
    std::string toString() const;

    /**
     * Creates an Any instance from a string value and type
     *
     * @param sValue Value as a string
     * @param type Type interpret the value as
     */
    static Any create(std::string sValue, Type type);

  private:
    Type type_;
    union
    {
      std::int64_t i64Value_;
      std::uint64_t u64Value_;
      double dValue_;
      std::string sValue_;
    };
  };

  template<typename T>
  struct AnyConvertableType
  {
    static Any::Type type();
  };

  template<typename T>
  struct is_any_convertable_nonnumeric
    : public std::false_type{};

  template<>
  struct is_any_convertable_nonnumeric<std::string>
    : public std::true_type{};

  std::string anyTypeAsString(const Any::Type & type);
}

#endif // OPENSTATISTIC_ANY_HEADER_
