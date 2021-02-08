# pull from our opentestpoint image
FROM gcr.io/automated-builds-303920/fedora33.opentestpoint

RUN dnf -y update
RUN dnf clean all

WORKDIR /opt
RUN git clone https://github.com/adjacentlink/openstatistic -b develop

WORKDIR openstatistic
RUN ./autogen.sh
RUN ./configure
RUN make rpm
RUN dnf -y install $(find .rpmbuild/RPMS -name "*\.rpm")

RUN echo 'fedora33 complete'

