/*
 * Copyright (c) 2015-2016 - Adjacent Link LLC, Bridgewater, New Jersey
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
#include "session.h"
#include "ostatistic/exception.h"
#include "registrarimpl.h"
#include "inetaddr.h"

#include <cstring>
#include <map>
#include <algorithm>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <thread>


class OpenStatistic::Service::Implementation
{
public:
  Implementation():
    iSignalEvent_{},
    iSock_{},
    thread_{},
    registrarImpl_{}{}

  void start(const std::string & sEndpoint)
  {
    INETAddr endpoint{sEndpoint};

    iSignalEvent_ = eventfd(0,0);

    if((iSock_ = socket(endpoint.getFamily(),
                        SOCK_STREAM,
                        0)) == -1)
      {
        throw Exception("%s",strerror(errno));
      }

    int iOption{1};

    if(setsockopt(iSock_,
                  SOL_SOCKET,
                  SO_REUSEADDR,
                  reinterpret_cast<void*>(&iOption),
                  sizeof(iOption)) < 0)
      {
        throw Exception("setsockopt SO_REUSEADDR: %s",
                        strerror(errno));

      }

    if(bind(iSock_,endpoint.getSockAddr(),endpoint.getAddrLength()) < 0)
      {
        throw Exception("bind: %s",
                        strerror(errno));
      }


    if(listen(iSock_,10) < 0)
      {
        throw Exception("listen: %s",
                        strerror(errno));
      }

    registrarImpl_.lock();

    thread_ = std::thread(&Implementation::process,this);
  }


  void stop()
  {
    eventfd_write(iSignalEvent_,1);
    thread_.join();
    ::close(iSock_);
    ::close(iSignalEvent_);
    registrarImpl_.unlock();
  }

  void process()
  {
    std::map<int,std::unique_ptr<Session>> sessionMap;

    while(1)
      {
        int nfds{iSignalEvent_};

        fd_set rfds;

        FD_ZERO(&rfds);
        FD_SET(iSignalEvent_,&rfds);
        FD_SET(iSock_,&rfds);

        nfds = std::max(iSock_,nfds);

        for(const auto & entry : sessionMap)
          {
            FD_SET(entry.first,&rfds);
            nfds = std::max(entry.first,nfds);
          }

        auto result = select(nfds+1,&rfds,nullptr,nullptr,nullptr);

        if(result == -1)
          {
            if(errno == EINTR)
              {
                continue;
              }
            else
              {
                break;
              }
          }

        if(FD_ISSET(iSignalEvent_,&rfds))
          {
            break;
          }

        if(FD_ISSET(iSock_,&rfds))
          {
            int iNewFd{};

            if((iNewFd = accept(iSock_,nullptr,nullptr)) > 0)
              {
                sessionMap.insert(std::make_pair(iNewFd,
                                                 std::unique_ptr<Session>{new Session{registrarImpl_}}));
              }
          }

        auto iter = sessionMap.begin();

        while(iter !=  sessionMap.end())
          {
            if(FD_ISSET(iter->first,&rfds))
              {
                // process the session data
                if(iter->second->process(iter->first))
                  {
                    ::close(iter->first);
                    sessionMap.erase(iter++);
                  }
                else
                  {
                    ++iter;
                  }
              }
            else
              {
                ++iter;
              }
          }
      }

    for(const auto & entry : sessionMap)
      {
        ::close(entry.first);
      }
  }

  Registrar & registrar()
  {
    return registrarImpl_;
  }

private:
  int iSignalEvent_;
  int iSock_{};
  std::thread thread_;
  RegistrarImpl registrarImpl_;
};

OpenStatistic::Service::Service():
  pImpl_{new Implementation{}}{}

void OpenStatistic::Service::start(const std::string & sEndpoint)
{
  pImpl_->start(sEndpoint);
}

void OpenStatistic::Service::stop()
{
  pImpl_->stop();
}

OpenStatistic::Registrar & OpenStatistic::Service::registrar()
{
  return pImpl_->registrar();
}

OpenStatistic::Service * OpenStatistic::Service::instance()
{
  static Service * pInstance{new Service{}};
  return pInstance;
}
