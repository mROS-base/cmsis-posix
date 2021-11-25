FROM ubuntu:20.04 as cmsis-posix

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
	git	\
	build-essential	\
	wget	\
	gcc	\
	g++	\
	vim	\
	libssl-dev libreadline-dev zlib1g-dev \
	make	\
	autoconf \
	automake \
	pkg-config \
	curl \
	net-tools \
	netcat \
	&& apt-get clean

RUN apt update
RUN apt upgrade

RUN apt-get install -y cmake
RUN apt-get install -y libgtest-dev lcov

#TODO REMOVE HERE
RUN useradd -ms /bin/bash tmori
RUN mkdir /home/tmori/.ssh
ADD id_rsa /home/tmori/.ssh/
RUN chmod 600 /home/tmori/.ssh/*
RUN chown tmori /home/tmori/.ssh
RUN chgrp tmori /home/tmori/.ssh
RUN chown tmori /home/tmori/.ssh/*
RUN chgrp tmori /home/tmori/.ssh/*

#TODO REMOVE HERE
USER tmori
WORKDIR /home/tmori
RUN ssh -o StrictHostKeyChecking=no -T git@github.com || true

#TODO for public change
RUN git clone git@github.com:mROS-base/cmsis-posix.git
