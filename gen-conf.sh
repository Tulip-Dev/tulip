#!/bin/sh

if [ -f /usr/bin/libtoolize ]; then
  LIBTOOLIZE=/usr/bin/libtoolize
else
  LIBTOOLIZE=/usr/bin/glibtoolize
fi 

echo "Creating configure for main package"
for CMD in "${LIBTOOLIZE} -c -f" "aclocal" "autoheader" "automake -a -c" "autoconf"
do
  echo "Running $CMD"
  $CMD
done
