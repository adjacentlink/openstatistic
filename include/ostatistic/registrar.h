/*
 * Copyright (c) 2013,2016 - Adjacent Link LLC, Bridgewater, New Jersey
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

#ifndef OPENSTATISTIC_REGISTRAR_HEADER_
#define OPENSTATISTIC_REGISTRAR_HEADER_

#include "ostatistic/statisticproperties.h"
#include "ostatistic/statisticnumeric.h"
#include "ostatistic/statisticnonnumeric.h"
#include "ostatistic/table.h"
#include "ostatistic/tablepublisher.h"

#include <string>
#include <memory>

namespace OpenStatistic
{
  class Registrar
  {
  public:
    virtual ~Registrar(){};

    /**
     * Register a numeric statistic. The registered statistic
     * is owned by the Registrar.
     *
     * @param sName Name of the statistic
     * @param properties Statistic properties
     * @param sDescription Statistic description
     *
     * @return A borrowed reference to the statistic that may
     * be used during the lifetime of the registered component.
     *
     * @throw RegistrarException when a error occurs during
     * registration.
     */
    template<typename T>
    StatisticNumeric<T> * registerNumeric(const std::string & sName,
                                          const StatisticProperties & properties = StatisticProperties::NONE,
                                          const std::string & sDescription = "");

    /**
     * Register a non-numeric statistic. The registered statistic
     * is owned by the Registrar.
     *
     *
     * @param sName Name of the statistic
     * @param properties Statistic properties
     * @param sDescription Statistic description
     *
     * @return A borrowed reference to the statistic that may
     * be used during the lifetime of the registered component.
     *
     * @throw RegistrarException when a error occurs during
     * registration.
     */
    template<typename T>
    StatisticNonNumeric<T> * registerNonNumeric(const std::string & sName,
                                                const StatisticProperties & properties = StatisticProperties::NONE,
                                                const std::string & sDescription = "");


    /**
     * Register a statistic table. The registered statistic table
     * is owned by the Registrar.
     *
     * @tparam Key Type of the keys. Each row in the table is uniquely
     *  identified by its key value
     * @tparam Compare A binary predicate that takes two element
     *  keys as arguments and returns a bool
     * @tparam scolumn The column used when sorting the table using
     *  Compare
     *
     * @param sName Name of the statistic table
     * @param labels Table column labels
     * @param properties Table properties
     * @param sDescription Statistic table description
     *
     * @return A borrowed reference to the statistic table that may
     *  be used during the lifetime of the registered component.
     *
     * @throw RegistrarException when a error occurs during
     *  registration.
     */
    template<typename Key,
             typename Compare = std::less<OpenStatistic::Any>,
             std::size_t scolumn = 0>
    Table<Key,Compare,scolumn> *
    registerTable(const std::string & sName,
                  const TableLabels & labels,
                  const StatisticProperties & properties = StatisticProperties::NONE,
                  const std::string & sDescription = "");


    /**
     * Register a statistic table. The registered statistic table
     * is owned by the Registrar.
     *
     * @tparam Key Type of the keys. Each row in the table is uniquely
     *  identified by its key value
     * @tparam Function A function that takes a pointer to the statistic
     *  table and that is called when the table is cleared
     * @tparam Compare A binary predicate that takes two element
     *  keys as arguments and returns a bool
     * @tparam scolumn The column used when sorting the table using
     *  Compare
     *
     * @param sName Name of the statistic table
     * @param labels Table column labels
     * @param clearFunc Function called when table clear is requested
     * @param sDescription Statistic table description
     *
     * @return A borrowed reference to the statistic table that may
     *  be used during the lifetime of the registered component.
     *
     * @throw RegistrarException when a error occurs during
     *  registration.
     *
     * @note The specified clear function is called on an internal emulator framework thread
     * a synchronization object will most likely me needed to avoid race conditions. The
     * function is responsible for clearing the statistic table.
     */
    template<typename Key,
             typename Function,
             typename Compare = std::less<OpenStatistic::Any>,
             std::size_t scolumn = 0>
    Table<Key,Compare,scolumn> *
    registerTable(const std::string & sName,
                  const TableLabels & labels,
                  Function clearFunc,
                  const std::string & sDescription = "");

  protected:
    Registrar() = default;

    /**
     * Register a statistic and take ownership
     *
     * @param sName Name of the statistic
     * @param type Any type of the statistic
     * @param properties Statistic properties
     * @param sDescription Statistic description
     * @param pStatistic Pointer to the statistic
     *
     * @throw RegistrarException when a error occurs during
     * registration.
     */
    virtual void registerStatistic(const std::string & sName,
                                   Any::Type type,
                                   const StatisticProperties & properties,
                                   const std::string & sDescription,
                                   Statistic * pStatistic) = 0;


    /**
     * Register a statistic table and take ownership
     *
     * @param sName Name of the statistic table
     * @param properties Statistic properties
     * @param sDescription Statistic table description
     * @param pTablePublisher Pointer to the statistic table publisher
     * @param clearFunc Function object to excute as part of clear
     *
     * @throw RegistrarException when a error occurs during
     * registration.
     */
    virtual void registerTablePublisher(const std::string & sName,
                                        const StatisticProperties & properties,
                                        const std::string & sDescription,
                                        TablePublisher * pTablePublisher,
                                        std::function<void(TablePublisher *)> clearFunc) = 0;
  };
}

#include "ostatistic/registrar.inl"

#endif // OPENSTATISTIC_REGISTRAR_HEADER_
