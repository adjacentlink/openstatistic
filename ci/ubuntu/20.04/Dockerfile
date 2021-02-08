# pull from our opentestpoint image
FROM gcr.io/automated-builds-303920/ubuntu20.04.opentestpoint

# prevent failures due to interactive apt transactions
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get -y update
RUN apt-get -y upgrade --no-install-recommends
RUN apt-get -y -f install

WORKDIR /opt
RUN git clone https://github.com/adjacentlink/openstatistic -b develop

WORKDIR openstatistic
RUN ./autogen.sh
RUN ./configure
RUN make deb
RUN dpkg -i $(find .debbuild -name "*\.deb") && apt-get -y -f install

RUN echo 'ubuntu20.04 complete'

