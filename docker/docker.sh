#!/bin/bash


TAG=tulip/tulip5

case $1 in
	build)
		docker build -t $TAG .
		;;

	run)
		docker run --rm -i -t t $TAG
	;;
esac
