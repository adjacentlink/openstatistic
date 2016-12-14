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

#include "ostatistic/exception.h"

template <typename T>
OpenStatistic::StatisticNumeric<T> *
OpenStatistic::Registrar::registerNumeric(const std::string & sName,
                                          const StatisticProperties & properties,
                                          const std::string & sDescription)
{
  static_assert(std::is_integral<T>() || std::is_floating_point<T>(),
                "Type not convertable to numeric Any");

  auto pStatistic = new StatisticNumeric<T>();

  registerStatistic(sName,
                    AnyConvertableType<T>::type(),
                    properties,
                    sDescription,
                    pStatistic);

  return pStatistic;
}


template <typename T>
OpenStatistic::StatisticNonNumeric<T> *
OpenStatistic::Registrar::registerNonNumeric(const std::string & sName,
                                             const StatisticProperties & properties,
                                             const std::string & sDescription)
{
  static_assert(is_any_convertable_nonnumeric<T>(),"Type not convertable to non-numeric Any");

  auto pStatistic = new StatisticNonNumeric<T>{};

  registerStatistic(sName,
                    AnyConvertableType<T>::type(),
                    properties,
                    sDescription,
                    pStatistic);

  return pStatistic;
}

template<typename Key, typename Compare, std::size_t scolumn>
OpenStatistic::Table<Key,Compare,scolumn> *
OpenStatistic::Registrar::registerTable(const std::string & sName,
                                        const TableLabels & labels,
                                        const StatisticProperties & properties,
                                        const std::string & sDescription)
{
  auto pTable = new Table<Key,Compare,scolumn>(labels);

  if(scolumn >= labels.size())
    {
      throw Exception("table sort column index out of range for: %s",
                      sName.c_str());
    }

  registerTablePublisher(sName,
                         properties,
                         sDescription,
                         pTable,
                         [](TablePublisher * p){p->clear();});

  return pTable;
}
template<typename Key, typename Function, typename Compare, std::size_t scolumn>
OpenStatistic::Table<Key,Compare,scolumn> *
OpenStatistic::Registrar::registerTable(const std::string & sName,
                                        const TableLabels & labels,
                                        Function clearFunc,
                                        const std::string & sDescription)
{
  auto pTable = new Table<Key,Compare,scolumn>(labels);

  if(scolumn >= labels.size())
    {
      throw Exception("table sort column index out of range for: %s",
                      sName.c_str());
    }

  registerTablePublisher(sName,
                         StatisticProperties::CLEARABLE,
                         sDescription,
                         pTable,
                         clearFunc);

  return pTable;
}
