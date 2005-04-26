#!/bin/sh


echo "Creating configure for main package"
for CMD in "libtoolize -c -f" "aclocal" "autoheader" "automake -a -c" "autoconf"
do
  echo "Running $CMD"
  $CMD
done
