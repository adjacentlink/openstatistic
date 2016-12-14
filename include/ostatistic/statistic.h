/*
 * Copyright (c) 2013-2014,2016 - Adjacent Link LLC, Bridgewater,
 * New Jersey
 * Copyright (c) 2010 - DRS CenGen, LLC, Columbia, Maryland
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
 * * Neither the name of DRS CenGen, LLC nor the names of its
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

#ifndef OPENSTATISTIC_STATISTIC_HEADER_
#define OPENSTATISTIC_STATISTIC_HEADER_

#include "ostatistic/any.h"

namespace OpenStatistic
{
  /**
   * @class Statistic
   *
   * @brief The Statistic interface is the base class
   * of all statistics. It provided methods to access
   * and clear the statistic.
   */
  class Statistic
  {
  public:
    virtual ~Statistic(){};

    /**
     * Gets a statistic value as an Any
     *
     * @return Statistic value as an Any
     */
    virtual Any asAny() const = 0;

    /**
     * Clears a statistic value
     */
    virtual void clear() = 0;

  protected:
    Statistic() = default;

  };
}

#endif // OPENSTATISTIC_STATISTIC_HEADER_
