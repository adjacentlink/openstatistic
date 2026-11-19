FROM fedora:45

RUN mkdir -p /opt/built

RUN dnf -y update








# Build tools
RUN dnf -y install git gcc-c++ make autoconf automake libtool rpm-build python3-setuptools python3-devel protobuf-devel

RUN git clone https://github.com/adjacentlink/openstatistic -b develop

WORKDIR openstatistic
RUN ./autogen.sh && ./configure && make rpm
RUN cp $(find .rpmbuild/RPMS -name "*\.rpm") /opt/built
RUN dnf -y install /opt/built/*

RUN dnf clean all

RUN echo 'complete'

