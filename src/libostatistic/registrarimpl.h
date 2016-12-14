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

#ifndef OPENSTATISTIC_REGISTRARIMPL_HEADER_
#define OPENSTATISTIC_REGISTRARIMPL_HEADER_

#include "ostatistic/registrar.h"
#include "statisticinfo.h"
#include "tableinfo.h"

#include <string>
#include <map>
#include <memory>
#include <mutex>

namespace OpenStatistic
{
  class RegistrarImpl : public Registrar
  {
  public:
    RegistrarImpl();

    ~RegistrarImpl();

    std::map<std::string,Any>
    queryStatistic(const std::vector<std::string> & names) const;

    void
    clearStatistic(const std::vector<std::string> & names) const;

    std::map<std::string,std::pair<TableLabels,TableValues>>
      queryTable(const std::vector<std::string> & names) const;

    void clearTable(const std::vector<std::string> & names) const;

    void lock();

    void unlock();

  private:
    using StatisticStore =
      std::map<std::string,std::pair<std::unique_ptr<Statistic>,StatisticInfo>>;

    using TableStore =
      std::map<std::string,std::tuple<std::unique_ptr<TablePublisher>,
                                      TableInfo,
                                      std::function<void(TablePublisher *)>>>;
    StatisticStore statisticStore_;
    TableStore tableStore_;
    bool bLocked_;
    std::mutex mutex_;

    void registerStatistic(const std::string & sName,
                           Any::Type type,
                           const StatisticProperties & properties,
                           const std::string & sDescription,
                           Statistic * pStatistic) override;

    void registerTablePublisher(const std::string & sName,
                                const StatisticProperties & properties,
                                const std::string & sDescription,
                                TablePublisher * pTablePublisher,
                                std::function<void(TablePublisher *)> clearFunc) override;

    RegistrarImpl(const RegistrarImpl &) = delete;
    RegistrarImpl & operator=(const RegistrarImpl &) = delete;
  };
}

#endif // OPENSTATISTIC_REGISTRARIMPL_HEADER_
