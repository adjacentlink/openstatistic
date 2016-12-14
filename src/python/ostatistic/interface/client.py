# Copyright (c) 2016 - Adjacent Link LLC, Bridgewater, New Jersey
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in
#   the documentation and/or other materials provided with the
#   distribution.
# * Neither the name of Adjacent Link LLC nor the names of its
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# See toplevel COPYING for more information.
#

import ostatistic.interface.openstatisticapi_pb2 as openstatisticapi_pb2
import socket
import struct

def fromAny(any):
    if any.type == openstatisticapi_pb2.Any.TYPE_ANY_INT64:
        return any.i64Value
    elif any.type == openstatisticapi_pb2.Any.TYPE_ANY_UINT64:
        return any.u64Value
    elif any.type == openstatisticapi_pb2.Any.TYPE_ANY_DOUBLE:
        return any.dValue
    elif any.type == openstatisticapi_pb2.Any.TYPE_ANY_STRING:
        return any.sValue
    elif any.type == openstatisticapi_pb2.Any.TYPE_ANY_BOOLEAN:
        return any.bValue

class ClientException(Exception):
    def __init__(self, value):
        self._value = str(value)
    def __str__(self):
        return self._value

class Client(object):
    def open(self,hostname,port):
        self._sequence = 0
        self._sock = socket.socket()
        self._sock.connect((hostname,port))

    def close(self):
        self._sock.close()
        self._sock = None

    def _transaction(self,request):
        msg = request.SerializeToString()

        self._sock.send(struct.pack("!L%ds" % len(msg),len(msg),msg))

        # wait for response
        buf = str()
        messageLengthBytes = 0
        response = None

        while True:
            if not messageLengthBytes:
                data = self._sock.recv(4-len(buf))

                if not len(data):
                    break

                if buf == None:
                    buf=data
                else:
                    buf+=data

                if(len(buf) == 4):
                    (messageLengthBytes,) = struct.unpack('!I',buf)
                    buf = str()

            else:
                data = self._sock.recv(messageLengthBytes-len(buf))

                if not len(data):
                    break

                if buf == None:
                    buf=data
                else:
                    buf+=data

                if(len(buf) == messageLengthBytes):
                    response = openstatisticapi_pb2.Response()
                    response.ParseFromString(buf)
                    messageLengthBytes = 0
                    buf = str()

                    if response.type == openstatisticapi_pb2.Response.TYPE_RESPONSE_ERROR:
                        raise ClientException(response.error.description);
                    else:
                        break

        return response

    def statistics(self,statistics=()):
        request = openstatisticapi_pb2.Request()

        request.sequence = self._sequence

        self._sequence += 1

        # set the Request type
        request.type = openstatisticapi_pb2.Request.TYPE_REQUEST_QUERY

        # set the Request Query type
        request.query.type = openstatisticapi_pb2.TYPE_QUERY_STATISTIC

        if statistics:
            for name in statistics:
                request.query.statistic.names.append(name)
        else:
            request.query.statistic.SetInParent()


        response = self._transaction(request)

        if response.type == openstatisticapi_pb2.Response.TYPE_RESPONSE_QUERY:
            if response.query.type == openstatisticapi_pb2.TYPE_QUERY_STATISTIC:
                statistics = {}
                for element in response.query.statistic.elements:
                    statistics[element.name] = fromAny(element.value)

                return statistics

            else:
                raise ClientException('malformed statistic query response')
        else:
            raise ClientException('malformed query response')

    def tables(self,tables=()):
        request = openstatisticapi_pb2.Request()

        request.sequence = self._sequence

        self._sequence += 1

        # set the Request type
        request.type = openstatisticapi_pb2.Request.TYPE_REQUEST_QUERY

        # set the Request Query type
        request.query.type = openstatisticapi_pb2.TYPE_QUERY_STATISTICTABLE

        if tables:
            for name in tables:
                request.query.statisticTable.names.append(name)
        else:
            request.query.statisticTable.SetInParent()

        response = self._transaction(request)

        if response.type == openstatisticapi_pb2.Response.TYPE_RESPONSE_QUERY:
            if response.query.type == openstatisticapi_pb2.TYPE_QUERY_STATISTICTABLE:
                tables = {}

                for table in response.query.statisticTable.tables:
                    tableData = []
                    for row in table.rows:
                        rowData = []
                        for value in row.values:
                            rowData.append(fromAny(value))
                        tableData.append(tuple(rowData))
                    tables[table.name] = (tuple(table.labels),tuple(tableData))

                return tables

            else:
                raise ClientException('malformed statistic table query response')
        else:
            raise ClientException('malformed query response')


    def clear_statistics(self,statistics=()):
        request = openstatisticapi_pb2.Request()

        request.sequence = self._sequence

        self._sequence += 1

        # set the Request type
        request.type = openstatisticapi_pb2.Request.TYPE_REQUEST_UPDATE

        # set the Request Update type
        request.update.type = openstatisticapi_pb2.TYPE_UPDATE_STATISTICCLEAR

        if statistics:
            for name in statistics:
                request.update.statisticClear.names.append(name)
        else:
            request.update.statisticClear.SetInParent()

        response = self._transaction(request)

        if response.type == openstatisticapi_pb2.Response.TYPE_RESPONSE_UPDATE:
            if response.update.type == openstatisticapi_pb2.TYPE_UPDATE_STATISTICCLEAR:
                return
            else:
                raise ClientException('malformed statistic update response')
        else:
            raise ClientException('malformed update response')


    def clear_tables(self,tables=()):
        request = openstatisticapi_pb2.Request()

        request.sequence = self._sequence

        self._sequence += 1

        # set the Request type
        request.type = openstatisticapi_pb2.Request.TYPE_REQUEST_UPDATE

        # set the Request Update type
        request.update.type = openstatisticapi_pb2.TYPE_UPDATE_STATISTICTABLECLEAR

        if tables:
            for name in tables:
                request.update.statisticTableClear.names.append(name)
        else:
            request.update.statisticTableClear.SetInParent()

        response = self._transaction(request)

        if response.type == openstatisticapi_pb2.Response.TYPE_RESPONSE_UPDATE:
            if response.update.type == openstatisticapi_pb2.TYPE_UPDATE_STATISTICTABLECLEAR:
                return
            else:
                raise ClientException('malformed statistic table update response')
        else:
            raise ClientException('malformed update response')
