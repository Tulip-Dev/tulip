#!/bin/bash


TAG=tulip:tulip5

case $1 in
	build)
		docker build -t $TAG .
		;;

	run)
		docker run \
			-t  \
			--privileged \
			--rm=true \
			-v "/tmp/.X11-unix:/tmp/.X11-unix:rw" \
			-e "DISPLAY=unix$DISPLAY" \
			-i \
			-e "TERM=xterm-256color" \
			-t \
			$TAG
		;;

	test)
		cd extend && \
			docker build -t tulip/test1 . && \
		docker run \
			-t  \
			--privileged \
			--rm=true \
			-v "/tmp/.X11-unix:/tmp/.X11-unix:rw" \
			-e "DISPLAY=unix$DISPLAY" \
			-i \
			-e "TERM=xterm-256color" \
			-t \
			$TAG
esac
