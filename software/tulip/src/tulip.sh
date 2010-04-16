#!/bin/sh

THISDIR=$(dirname $0)

export DYLD_LIBRARY_PATH=$THISDIR/../Frameworks
$THISDIR/tulip_app
