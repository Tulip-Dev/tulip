#!/bin/sh

echo "Converting Files to Unix format"
for CMD in `find . -name "*.*"`
do
  `dos2unix $CMD`
done
