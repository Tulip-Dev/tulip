#!/bin/sh

cd doc
for A in `find . -name "*.doxygen"`
do
  cd `dirname $A`
  doxygen `basename $A`
  cd -
done
sh gen-index.sh
cd ..
