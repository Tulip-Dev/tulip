#!/bin/bash
# first remove comments
#sed -n '/\/\//!p' $1 > $1.tmp

echo "// This file has been generated using $1"
echo ""
echo "static const char* fragmentShaderHeaderSource[] ="
echo "{"

while read line;
do
# when the file has been updated on windows
# the lines may have an additionnal Control-r at the end
# LEN=$((${#line} - 1));
# echo "\"${line:0:$LEN}\",";
  echo "\"$line\",";
done < $1

# null terminated
echo "0"
echo "};"
