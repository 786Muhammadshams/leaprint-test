#!/bin/bash

# This script is run within the buildah container in order to build an image
# from our Dockerfile. It is called by `rake image`, so there's no need to ever
# run it directly.
buildah bud --isolation=chroot -f Dockerfile .
