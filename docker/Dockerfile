FROM ubuntu:16.04
MAINTAINER Romain GIOT <romain.giot@u-bordeaux.fr>

ENV DEBIAN_FRONTEND noninteractive

# Install libraries and construction tools
RUN apt-get update && \
    apt-get install -y \
	binutils \
	binutils-dev \
	build-essential \
	cmake \
	git \
	kde-style-oxygen-qt4 \
	kde-style-qtcurve \
	imagemagick \
	libfdt1 \
	libfdt-dev \
	libfreetype6 \
	libfreetype6-dev \
	libglew1.13 \
	libglew-dev \
	libglewmx1.13 \
	libglewmx-dev \
	libglu1-mesa-dev  \
	libjpeg62 \
	libjpeg62-dev \
	libpng12-0 \
	libpng12-dev \
	libpng++-dev \
	libquazip1 \
	libquazip-dev \
	mesa-utils \
	qt5-default \
	pyqt5-dev-tools \
	python-dev \
    	subversion \
	wget \
	xserver-xorg-video-all\
	x-window-system \
	zlib1g \
	zlib1g-dev 


# Work with a user named docker
RUN useradd \
	--home-dir /home/docker \
	--create-home \
	--shell /bin/bash \
	docker
USER docker
RUN mkdir -p \
	/home/docker/src \
	/home/docker/build 

# Download Tulip 5 sources
WORKDIR /home/docker/src
RUN  git clone https://github.com/tulip5/tulip.git


# Compile whole tulip a first time in order to speed up recompilation for child container adding content in externalplugins
WORKDIR /home/docker/build
RUN cmake /home/docker/src/tulip* \
    	-DCMAKE_CXX_FLAGS="-std=gnu++11" \
	-DCMAKE_BUILD_TYPE=Release \
	-DBUILD_DOC=OFF && \
    make -j$(nproc) 



# Install it
USER root
RUN make -j8 install


# XXX Here is the sad par of docker ...
# Install video drivers (at the end to ensure to not rebuild from scratch each time we test a new driver)
# XXX Adapt this line to your own computer
RUN apt-get update && apt-get upgrade -qy nvidia-352

USER docker


CMD tulip --perspective Tulip
