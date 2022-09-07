#!/bin/bash

# This script starts up an ubuntu container which can be used to develop and
# build the LEA Print executable which will later be copied to the buildah
# container in order to build our image.
#
# You'll need to manually install the dependencies within the container in
# order to develop and build. See the Dockerfile for a list of dependencies.
#
# You can probably do this using `rake dev` but I haven't tested that yet.
docker run -d \
  -it \
  --name LEAPrintDev \
  --mount type=bind,source="$(pwd)",target=/app \
  ubuntu:jammy
