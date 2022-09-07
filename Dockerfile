# This Dockerfile is copied to the buildah container and then used to build an
# image with the LEAPrint server executable (already built using the `dxx`
# script) and its Dependencies.
FROM ubuntu:jammy
LABEL summary="LEA Print Server for generating Drive Order PDFs"

# Setup timezone to prevent hanging during package installation
ENV TZ=Europe/Zurich
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Dependencies
RUN apt-get update
#RUN apt-get install -y curl
RUN apt-get install -y libboost-dev libboost-locale-dev
#RUN apt-get install -y libexpat1-dev default-libmysqlclient-dev libpcre3-dev libsqlite3-dev libssl-dev zlib1g-dev # for poco
RUN apt-get install -y libpoco-dev
RUN apt-get install -y libcairo2-dev
RUN apt-get install -y curl

# Remove debug poco libraries
RUN rm -f /usr/local/lib/libPoco{Data,Foundation,Net,XML,Util,JSON,Zip,PDF,NetSSL,DataSQLite,Crypto,CppParser,JWT,Encodings,Redis,ActiveRecord}d.a

COPY a.out /bin/LEAPrintServer

RUN mkdir -p /usr/share
COPY fonts /usr/share/LEAPrintFonts

#RUN curl -o LEAPrintServer https://github.com/COMREX/LEAPrintServerBinaries/releases/download/1.0.0/LEAPrintServer
EXPOSE 9090
CMD /bin/LEAPrintServer
