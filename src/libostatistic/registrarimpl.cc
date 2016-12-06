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

#include "registrarimpl.h"
#include "ostatistic/exception.h"

#include <iterator>
#include <map>

OpenStatistic::RegistrarImpl::RegistrarImpl():
  bLocked_{}{}

OpenStatistic::RegistrarImpl::~RegistrarImpl(){}

void OpenStatistic::RegistrarImpl::registerStatistic(const std::string & sName,
                                                     Any::Type type,
                                                     const StatisticProperties & properties,
                                                     const std::string & sDescription,
                                                     Statistic * pStatistic)
{
  std::lock_guard<std::mutex> m(mutex_);

  if(bLocked_)
    {
      throw Exception("Unable to register a statistic while service is running.");
    }

  if(std::find_if_not(sName.begin(),sName.end(),[](int ch){return isalnum(ch) || ch == '.';}) != sName.end())
    {
      throw Exception("Invalid charater in the statistic name: %s",
                      sName.c_str());
    }

  StatisticInfo info{sName,
      type,
      properties,
      sDescription};

  std::unique_ptr<Statistic> pStat{pStatistic};

  auto iter = statisticStore_.find(sName);

  if(iter == statisticStore_.end())
    {
      statisticStore_.insert(std::make_pair(sName,std::make_pair(std::move(pStat),std::move(info))));
    }
  else
    {
      throw Exception("Statistic already registered: %s",
                      sName.c_str());
    }
}

void OpenStatistic::RegistrarImpl::registerTablePublisher(const std::string & sName,
                                                          const StatisticProperties & properties,
                                                          const std::string & sDescription,
                                                          TablePublisher * pTablePublisher,
                                                          std::function<void(TablePublisher *)> clearFunc)
{
  std::lock_guard<std::mutex> m(mutex_);

  if(bLocked_)
    {
      throw Exception("Unable to register a table while service is running.");
    }

  if(std::find_if_not(sName.begin(),sName.end(),[](int ch){return isalnum(ch) || ch == '.';}) != sName.end())
    {
      throw Exception("Invalid charater in the statistic table name: %s",
                      sName.c_str());
    }

  TableInfo info{sName,properties,sDescription};

  std::unique_ptr<TablePublisher> pTable{pTablePublisher};

  auto iter = tableStore_.find(sName);

  if(iter == tableStore_.end())
    {
      tableStore_.insert(std::make_pair(sName,std::make_tuple(std::move(pTable),
                                                              std::move(info),
                                                              std::move(clearFunc))));
    }
  else
    {
      throw Exception("Statistic table already registered: %s",
                      sName.c_str());
    }
}

void OpenStatistic::RegistrarImpl::lock()
{
  std::lock_guard<std::mutex> m(mutex_);
  bLocked_ = true;
}

void OpenStatistic::RegistrarImpl::unlock()
{
  std::lock_guard<std::mutex> m(mutex_);
  bLocked_ = false;
}

std::map<std::string,OpenStatistic::Any>
OpenStatistic::RegistrarImpl::queryStatistic(const std::vector<std::string> & names) const
{
  std::map<std::string,Any> values;

  if(names.empty())
    {
      std::transform(statisticStore_.begin(),
                     statisticStore_.end(),
                     std::inserter(values,values.end()),
                     [](const StatisticStore::value_type & p)
                     {
                       return std::make_pair(p.first,p.second.first->asAny());
                     });

    }
  else
    {
      for(const auto & name : names)
        {
          auto iter = statisticStore_.lower_bound(name);

          for(; iter != statisticStore_.end(); ++iter)
            {
              if(iter->first.compare(0,name.size(),name) == 0)
                {
                  values.insert(std::make_pair(iter->first,
                                               iter->second.first->asAny()));
                }
              else
                {
                  break;
                }
            }

          if(values.empty())
            {
              throw Exception("Unknown statistic name: %s", name.c_str());
            }
        }
    }

  return values;
}
void
OpenStatistic::RegistrarImpl::clearStatistic(const std::vector<std::string> & names) const
{
  if(names.empty())
    {
      std::for_each(statisticStore_.begin(),
                    statisticStore_.end(),
                    [](const StatisticStore::value_type & p)
                    {
                      if(p.second.second.isClearable())
                        {
                          p.second.first->clear();
                        }
                    });

    }
  else
    {
      std::vector<Statistic *> statsToClear;

      // store references to all the requested statistics to clear
      // this a transactional API call so either all requested items
      // are valid names and clearable or none will be cleared
      for(const auto & name : names)
        {
          auto iter = statisticStore_.lower_bound(name);

          for(; iter != statisticStore_.end(); ++iter)
            {
              if(iter->first.compare(0,name.size(),name) == 0)
                {
                  if(iter->second.second.isClearable())
                    {
                      statsToClear.push_back(iter->second.first.get());
                    }
                  else
                    {
                      throw Exception("Statistic not clearable: %s",
                                      iter->first.c_str());
                    }
                }
              else
                {
                  break;
                }
            }

          if(statsToClear.empty())
            {
              throw Exception("Unknown statistic name: %s", name.c_str());
            }
        }

      for_each(statsToClear.begin(),
               statsToClear.end(),
               bind(&Statistic::clear,
                    std::placeholders::_1));
    }
}


std::map<std::string,std::pair<OpenStatistic::TableLabels,OpenStatistic::TableValues>>
  OpenStatistic::RegistrarImpl::queryTable(const std::vector<std::string> & names) const
{
  std::map<std::string,std::pair<TableLabels,TableValues>> values;

  if(names.empty())
    {
      std::transform(tableStore_.begin(),
                     tableStore_.end(),
                     std::inserter(values,values.end()),
                     [](const TableStore::value_type & p)
                     {
                       auto pTable = std::get<0>(p.second).get();
                       return std::make_pair(p.first,
                                             std::make_pair(pTable->getLabels(),
                                                            pTable->getValues()));
                     });

    }
  else
    {
      for(const auto & name : names)
        {
          auto iter = tableStore_.lower_bound(name);

          for(; iter != tableStore_.end(); ++iter)
            {
              if(iter->first.compare(0,name.size(),name) == 0)
                {
                  auto pTable = std::get<0>(iter->second).get();

                  values.insert(std::make_pair(iter->first,
                                               std::make_pair(pTable->getLabels(),
                                                              pTable->getValues())));
                }
              else
                {
                  break;
                }
            }

          if(values.empty())
            {
              throw Exception("Unknown table name: %s", name.c_str());
            }
        }
    }

  return values;
}

void OpenStatistic::RegistrarImpl::clearTable(const std::vector<std::string> & names) const

{
  // store references to all the requested statistics to clear
  // this a transactional API call so either all requested items
  // are valid names and clearable or none will be cleared
  std::vector<std::function<void()>> tablesToClear;

  if(names.empty())
    {
      std::for_each(tableStore_.begin(),
                    tableStore_.end(),
                    [&tablesToClear](const TableStore::value_type & p)
                    {
                      if(std::get<1>(p.second).isClearable())
                        {
                          tablesToClear.push_back(std::bind(std::get<2>(p.second),
                                                            std::get<0>(p.second).get()));
                        }
                    });

    }
  else
    {
      for(const auto & name : names)
        {
          auto iter = tableStore_.lower_bound(name);

          for(; iter != tableStore_.end(); ++iter)
            {
              if(iter->first.compare(0,name.size(),name) == 0)
                {
                  if(std::get<1>(iter->second).isClearable())
                    {
                      tablesToClear.push_back(std::bind(std::get<2>(iter->second),
                                                        std::get<0>(iter->second).get()));
                    }
                  else
                    {
                      throw Exception("Table not clearable: %s",iter->first.c_str());
                    }
                }
              else
                {
                  break;
                }
            }

          if(tablesToClear.empty())
            {
              throw Exception("Unknown table name: %s", name.c_str());
            }
        }
    }

  for_each(tablesToClear.begin(),
           tablesToClear.end(),
           [](const std::function<void()> & f)
           {f();});

}
