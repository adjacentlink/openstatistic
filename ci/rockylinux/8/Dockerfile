FROM rockylinux:8

RUN mkdir -p /opt/built

RUN dnf -y update
RUN dnf -y install epel-release dnf-plugins-core
RUN dnf config-manager --set-enabled powertools
RUN dnf clean all







# Build tools
RUN dnf -y install git gcc-c++ make autoconf automake libtool rpm-build

RUN git clone https://github.com/adjacentlink/openstatistic -b develop

WORKDIR openstatistic
RUN ./autogen.sh && ./configure && make rpm
RUN cp $(find .rpmbuild/RPMS -name "*\.rpm") /opt/built
RUN dnf -y install /opt/built/*

RUN dnf clean all

RUN echo 'complete'

