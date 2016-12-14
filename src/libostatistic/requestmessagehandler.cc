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

#include "requestmessagehandler.h"
#include "queryhandler.h"
#include "tablequeryhandler.h"
#include "clearupdatehandler.h"
#include "tableclearupdatehandler.h"
#include "errorresponse.h"
#include "openstatisticapi.pb.h"

std::string
OpenStatistic::RequestMessageHandler::process(const OpenStatisticAPI::Request & request,
                                              std::uint32_t u32Sequence,
                                              RegistrarImpl & registrarImpl)

{
  if(request.type() ==  OpenStatisticAPI::Request::TYPE_REQUEST_QUERY)
    {
      if(request.has_query())
        {
          const auto & query = request.query();

          if(query.type() == OpenStatisticAPI::TYPE_QUERY_STATISTIC)
            {
              if(query.has_statistic())
                {
                  return QueryHandler::process(query.statistic(),
                                               u32Sequence,
                                               request.sequence(),
                                               registrarImpl);

                }
              else
                {
                  return ErrorResponse::serialize(OpenStatisticAPI::Response::Error::TYPE_ERROR_MALFORMED,
                                                  "Statistic query request malformed: missing statistic message",
                                                  u32Sequence,
                                                  request.sequence());
                }
            }
          else if(query.type() == OpenStatisticAPI::TYPE_QUERY_STATISTICTABLE)
            {
              if(query.has_statistictable())
                {
                  return TableQueryHandler::process(query.statistictable(),
                                                    u32Sequence,
                                                    request.sequence(),
                                                    registrarImpl);
                }
              else
                {
                  return ErrorResponse::serialize(OpenStatisticAPI::Response::Error::TYPE_ERROR_MALFORMED,
                                                  "Statistic Table query request malformed: missing statisticTable message",
                                                  u32Sequence,
                                                  request.sequence());

                }
            }
        }
      else
        {
          ErrorResponse::serialize(OpenStatisticAPI::Response::Error::TYPE_ERROR_MALFORMED,
                                   "Query request malformed: missing query message",
                                   u32Sequence,
                                   request.sequence());

        }
    }
  else if(request.type() == OpenStatisticAPI::Request::TYPE_REQUEST_UPDATE)
    {
      if(request.has_update())
        {
          const auto & update = request.update();

          if(update.type() == OpenStatisticAPI::TYPE_UPDATE_STATISTICCLEAR)
            {
              if(update.has_statisticclear())
                {
                  return ClearUpdateHandler::process(update.statisticclear(),
                                                     u32Sequence,
                                                     request.sequence(),
                                                     registrarImpl);

                }
              else
                {
                  return ErrorResponse::serialize(OpenStatisticAPI::Response::Error::TYPE_ERROR_MALFORMED,
                                                  "Statistic Table update request malformed: missing statisticClear message",
                                                  u32Sequence,
                                                  request.sequence());
                }
            }
          else if(update.type() == OpenStatisticAPI::TYPE_UPDATE_STATISTICTABLECLEAR)
            {
              if(update.has_statistictableclear())
                {
                  return TableClearUpdateHandler::process(update.statistictableclear(),
                                                          u32Sequence,
                                                          request.sequence(),
                                                          registrarImpl);

                }
              else
                {
                  return ErrorResponse::serialize(OpenStatisticAPI::Response::Error::TYPE_ERROR_MALFORMED,
                                                  "Statistic Table Clear update request malformed: missing statisticTableClear message",
                                                  u32Sequence,
                                                  request.sequence());
                }
            }
        }
      else
        {
          return ErrorResponse::serialize(OpenStatisticAPI::Response::Error::TYPE_ERROR_MALFORMED,
                                          "Update request malformed: missing update message",
                                          u32Sequence,
                                          request.sequence());
        }
    }

  return ErrorResponse::serialize(OpenStatisticAPI::Response::Error::TYPE_ERROR_MALFORMED,
                                  "Request malformed: unknown request type",
                                  u32Sequence,
                                  request.sequence());
}
