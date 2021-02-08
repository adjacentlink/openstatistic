# pull from our opentestpoint image
FROM gcr.io/automated-builds-303920/centos7.opentestpoint

RUN yum -y update
RUN yum clean all

WORKDIR /opt
RUN git clone https://github.com/adjacentlink/openstatistic -b develop

WORKDIR openstatistic
RUN ./autogen.sh
RUN ./configure --with-python2
RUN make rpm
RUN yum -y install $(find .rpmbuild/RPMS -name "*\.rpm")

RUN echo 'centos7 complete'

