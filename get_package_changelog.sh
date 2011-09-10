#!/bin/bash 

echo "Enter package changelog :"
changelog=""
read input
while [ "$input" != "" ]
do
  if [ "$changelog" = "" ]
  then
    changelog="$input"
  else
    changelog="$changelog\n$input"
  fi
  read input
done

echo "$changelog" 1>&2
