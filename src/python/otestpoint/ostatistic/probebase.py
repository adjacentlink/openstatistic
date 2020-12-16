#
# Copyright (c) 2014-2016,2019 - Adjacent Link LLC, Bridgewater,
# New Jersey
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#  * Neither the name of Adjacent Link LLC nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
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

from pkg_resources import resource_filename
from otestpoint.interface import Probe
from otestpoint.interface import ProbeException
from otestpoint.interface.measurementtable_pb2 import MeasurementTable
import otestpoint.toolkit.logger as Logger
import ostatistic.interface.openstatisticapi_pb2 as openstatisticapi_pb2

from lxml import etree
import re
import sys
import socket
import struct

def fromAny(any):
    if any.type == openstatisticapi_pb2.Any.TYPE_ANY_INT64:
        return (any.i64Value,any.type)
    elif any.type == openstatisticapi_pb2.Any.TYPE_ANY_UINT64:
        return (any.u64Value,any.type)
    elif any.type == openstatisticapi_pb2.Any.TYPE_ANY_DOUBLE:
        return (any.dValue,any.type)
    elif any.type == openstatisticapi_pb2.Any.TYPE_ANY_STRING:
        return (any.sValue,any.type)
    elif any.type == openstatisticapi_pb2.Any.TYPE_ANY_BOOLEAN:
        return (any.bValue,any.type)


def loadMeasurement(m,value,atype):
    if atype == openstatisticapi_pb2.Any.TYPE_ANY_INT64:
        m.type = MeasurementTable.Measurement.TYPE_SINTEGER
        m.iValue = value
    elif atype == openstatisticapi_pb2.Any.TYPE_ANY_UINT64:
        m.type = MeasurementTable.Measurement.TYPE_UINTEGER
        m.uValue = value
    elif atype == openstatisticapi_pb2.Any.TYPE_ANY_DOUBLE:
        m.type = MeasurementTable.Measurement.TYPE_DOUBLE
        m.dValue = value
    elif atype == openstatisticapi_pb2.Any.TYPE_ANY_STRING:
        m.type = MeasurementTable.Measurement.TYPE_STRING
        m.sValue = value
    elif atype == openstatisticapi_pb2.Any.TYPE_ANY_BOOLEAN:
        m.type = MeasurementTable.Measurement.TYPE_UINTEGER
        m.uValue = value

class ProbeBase(Probe):
    def __init__(self,name,moduleName,schemaFile):
        self._name = name
        self._moduleName = moduleName
        self._schemaFile = schemaFile
        self._sequence = 0
        self._sock = None

    def build(self,probe):
        raise NotImplemented

    def loadTable(self,member,table,data):
        return None

    def initialize(self,configurationFile):
        self._logger.log(Logger.DEBUG_LEVEL,"/%s initialize configuration: %s" % (self._name,configurationFile))

        tree = etree.parse(configurationFile)

        root = tree.getroot()

        schemaDoc = etree.parse(resource_filename(self._moduleName,self._schemaFile))

        schema = etree.XMLSchema(etree=schemaDoc,attribute_defaults=True)

        if not schema(root):
            message = ""
            for entry in schema.error_log:
                message += "%d: %s " % (entry.line,entry.message)
            self._logger.log(Logger.ERROR_LEVEL,"/%s %s" % (self._name,message))
            raise ProbeException(message)

        self._address = root.get("address")
        self._port = int(root.get("port"))

        self._logger.log(Logger.DEBUG_LEVEL,
                         "/%s initialize contol port address %s port %d" % (self._name,
                                                                            self._address,
                                                                            self._port))
        m = None
        self._probes = []
        self._tables = []

        names = []

        for probe in root.xpath("//*/probes/*[@enable='yes']"):
            names.append(probe.tag)

            normalize = probe.tag.replace(".","_").lower()

            p = self.build("Measurement_%s"%normalize)

            match = re.match(".+\.Tables\..+",probe.tag)

            if match:
                self._tables.append((probe.tag,p))
            else:
                self._probes.append((probe.tag,p))

        return names

    def start(self):
        self._logger.log(Logger.DEBUG_LEVEL,"/%s start" % self._name)

        self._sock = socket.socket()

        try:
            self._sock.connect((self._address,self._port))
        except:
            raise ProbeException("unable to connect to %s:%d" % (self._address,self._port))

        self._logger.log(Logger.DEBUG_LEVEL,"/%s control port connected" % self._name)

    def stop(self):
        self._logger.log(Logger.DEBUG_LEVEL,"/%s stop" % self._name )
        self._sock.close()

    def destroy(self):
        self._logger.log(Logger.DEBUG_LEVEL,"/%s destroy" % self._name)

    def probe(self):
        self._logger.log(Logger.DEBUG_LEVEL,"/%s probe" % self._name)
        request = openstatisticapi_pb2.Request()
        request.type = openstatisticapi_pb2.Request.TYPE_REQUEST_QUERY
        request.query.type = openstatisticapi_pb2.TYPE_QUERY_STATISTIC
        request.query.statistic.SetInParent()
        self._logger.log(Logger.DEBUG_LEVEL,"/%s probe getting statistics" % self._name)
        response = self._transaction(request)

        stats = {}

        if response.type == openstatisticapi_pb2.Response.TYPE_RESPONSE_QUERY:
            if response.query.type == openstatisticapi_pb2.TYPE_QUERY_STATISTIC:
                for element in response.query.statistic.elements:
                    stats[element.name] = fromAny(element.value)[0]

        request = openstatisticapi_pb2.Request()
        request.type = openstatisticapi_pb2.Request.TYPE_REQUEST_QUERY
        request.query.type = openstatisticapi_pb2.TYPE_QUERY_STATISTICTABLE
        request.query.statisticTable.SetInParent()
        self._logger.log(Logger.DEBUG_LEVEL,"/%s probe getting tables" % self._name)
        response = self._transaction(request)

        tables = {}

        if response.type == openstatisticapi_pb2.Response.TYPE_RESPONSE_QUERY:
            if response.query.type == openstatisticapi_pb2.TYPE_QUERY_STATISTICTABLE:
                for table in response.query.statisticTable.tables:
                    tableData = []
                    for row in table.rows:
                        rowData = []
                        for value in row.values:
                            rowData.append(fromAny(value))
                        tableData.append(tuple(rowData))
                    tables[table.name] = (tuple(table.labels),tuple(tableData))

        self._logger.log(Logger.DEBUG_LEVEL,"/%s probe processing" % self._name)
        lstats = {}

        for name,entry in list(stats.items()):
            lstats[name.lower()] = entry

        ltables = {}

        for name,entry in list(tables.items()):
            ltables[name.lower()] = entry

        probeData = []

        for name,probe in self._probes:
            probe.Clear()
            for member in sorted(probe.DESCRIPTOR.fields_by_name.keys()):
                if member != 'description':
                    setattr(probe,member,lstats[member.replace('_','.')])

            probeData.append((name,
                              probe.SerializeToString(),
                              probe.description.name,
                              probe.description.module,
                              probe.description.version))


        for name,probe in self._tables:
            probe.Clear()

            for member in sorted(probe.DESCRIPTOR.fields_by_name.keys()):
                if member != 'description':
                    table = getattr(probe,member)

                    labels,rows = ltables[member.replace('_','.')]

                    if not self.loadTable(member,table,rows):
                        for label in labels:
                            table.labels.append(label)

                        for row in rows:
                            pass
                            entries = []
                            r = table.rows.add()
                            for entry in row:
                                e = r.values.add()
                                loadMeasurement(e,*entry)

            probeData.append((name,
                              probe.SerializeToString(),
                              probe.description.name,
                              probe.description.module,
                              probe.description.version))

        return probeData

    def _transaction(self,request):
        # send request
        self._sequence+=1

        request.sequence = self._sequence

        msg = request.SerializeToString()

        self._sock.send(struct.pack("!L%ds" % len(msg),len(msg),msg))

        # wait for response
        buf = bytes()
        messageLengthBytes = 0
        running = True
        response = None

        while True:
            if not messageLengthBytes:
                data = self._sock.recv(4-len(buf))

                if not len(data):
                    running = False
                    break

                if buf == None:
                    buf=data
                else:
                    buf+=data

                if(len(buf) == 4):
                    (messageLengthBytes,) = struct.unpack('!I',buf)
                    buf = bytes()

            else:
                data = self._sock.recv(messageLengthBytes-len(buf))

                if not len(data):
                    running = False
                    break

                if buf == None:
                    buf=data
                else:
                    buf+=data

                if(len(buf) == messageLengthBytes):
                    response = openstatisticapi_pb2.Response()
                    response.ParseFromString(buf)
                    messageLengthBytes = 0
                    buf = bytes()
                    break

        return response
