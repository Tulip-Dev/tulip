#!/bin/bash 

echo "Enter package changelog :"
changelog=""
read input
while [ "$input" != "" ]
do
  if [ "$changelog" = "" ]
  then
    changelog="\t$input"
  else
    changelog="$changelog\n\t$input"
  fi
  read input
done

echo -e "$changelog" 1>&2
