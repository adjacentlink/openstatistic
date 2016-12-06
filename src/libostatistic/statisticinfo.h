/*
 * Copyright (c) 2013-2014,2016 - Adjacent Link LLC, Bridgewater,
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

#ifndef OPENSTATISTIC_STATISTICINFO_HEADER_
#define OPENSTATISTIC_STATISTICINFO_HEADER_

#include "ostatistic/statistic.h"
#include "ostatistic/statisticproperties.h"

#include <string>
#include <memory>

namespace OpenStatistic
{
  /**
   * @class StatisticInfo
   *
   * @brief Holds name, type, property and description of
   * a Statistic.
   */
  class StatisticInfo
  {
  public:
    /**
     * Creates a StatisticInfo object to hold information
     * about a registered statistic
     *
     * @param sName Statistic name
     * @param type Statistic Any type
     * @param properties Statistic properties
     * @param sDescription Statsitci description
     */
    StatisticInfo(const std::string & sName,
                  Any::Type type,
                  const StatisticProperties & properties,
                  const std::string & sDescription);

    /**
     * Gets the statistic name
     *
     * @return Returns a const reference statistic name
     * reference
     */
    const std::string & getName() const;


    /**
     * Get the statistic Any type
     *
     * @return Statistic Any type
     */
    Any::Type getType() const;

    /**
     * Gets the stastitic description
     *
     * @return  Returns a const reference description
     */
    const std::string & getDescription() const;

    /**
     * Is the statistic clearable
     *
     * @return boolean flag
     */
    bool isClearable() const;

  private:
    std::string sName_;
    Any::Type type_;
    StatisticProperties properties_;
    std::string sDescription_;
  };
}

#include "statisticinfo.inl"

#endif // OPENSTATISTIC_STATISTICINFO_HEADER_
