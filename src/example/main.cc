/*
 * Copyright (c) 2016 - Adjacent Link LLC, Bridgewater, New Jersey
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

#include "ostatistic/service.h"

#include <iostream>
#include <exception>
#include <signal.h>
#include <cstring>
#include <atomic>
#include <unistd.h>

namespace
{
  std::atomic<bool> bRun{true};

  void sighandler(int)
  {
    bRun = false;
  }
}

int main(int , char * [])
{
  try
    {
      // Obtain a reference to the OpenStatistic registrar. This
      // reference is used to register statistics and tables. You can
      // use the OpenStatistic::Service singleton throughout your
      // project to make it easy to add stats and tables.
      auto & registrar = OpenStatistic::Service::instance()->registrar();


      // create statistics and tables
      auto pCount1 =
        registrar.registerNumeric<std::uint64_t>("foo.bar.moo.count1",
                                                 OpenStatistic::StatisticProperties::CLEARABLE);

      auto pCount2 =
        registrar.registerNumeric<std::uint64_t>("foo.bar.moo.count2",
                                                 OpenStatistic::StatisticProperties::CLEARABLE);

      auto pCount3 =
        registrar.registerNumeric<std::uint64_t>("foo.bar.count3",
                                                 OpenStatistic::StatisticProperties::NONE);

      auto pMoo =
        registrar.registerNumeric<std::uint64_t>("moo",
                                                 OpenStatistic::StatisticProperties::NONE);

      auto pFooTable = registrar.registerTable<std::uint64_t>("foo.bar.moo.FooTable",
        {"Foo","Bar","Moo"},
        OpenStatistic::StatisticProperties::NONE,
        "FooTable description");

      for(unsigned i = 0; i < 10; ++i)
        {
          pFooTable->addRow(i,
                            {OpenStatistic::Any{static_cast<uint64_t>(i)},
                                OpenStatistic::Any{static_cast<uint64_t>(i*2)},
                                  OpenStatistic::Any{"moo"}});
        }


      auto pBarTable = registrar.registerTable<std::uint64_t>("foo.bar.moo.BarTable",
        {"Bar","Foo","Moo"},
        OpenStatistic::StatisticProperties::NONE,
        "BarTable description");

      (void) pBarTable;

      // Start the OpenStatistic Service
      OpenStatistic::Service::instance()->start("localhost:47001");

      struct sigaction action;

      memset(&action,0,sizeof(action));

      action.sa_handler = sighandler;

      sigaction(SIGINT,&action,nullptr);
      sigaction(SIGQUIT,&action,nullptr);

      // Use your statistics and tables accordingly
      while(bRun)
        {
           ++*pCount1;
           ++*pCount2;
           ++*pCount3;
           ++*pMoo;

           pFooTable->setCell(1,1,OpenStatistic::Any{pCount3->get()});

           sleep(1);
        }

      OpenStatistic::Service::instance()->stop();
    }
  catch(std::exception & exp)
    {
      std::cerr<<exp.what()<<std::endl;
    }

  std::cout<<"bye"<<std::endl;
}
