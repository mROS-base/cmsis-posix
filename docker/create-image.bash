#!/bin/bash

DOCKER_IMAGE=`cat docker_image.txt`

cp /home/tmori/.ssh/id_rsa .
docker build -t ${DOCKER_IMAGE} -f Dockerfile .
rm -f id_rsa
