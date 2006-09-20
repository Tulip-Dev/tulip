FILES=$(find . -name CVS -prune -o -name "*.h" -print -o -name "*.cpp" -print -o -name "*.cxx" -print)

HEADER=$(cat ./header.txt)

for FILE in $FILES
do
  echo "new header for $FILE"
  # normalize header
  sed '1s/\/\/-\*-C.*/\/\/-\*-c\+\+-\*-/' $FILE > tmpfile
  mv tmpfile $FILE
  # output header
  echo '//-*-c++-*-' > tmpfile
  echo "$HEADER" >> tmpfile
  first=$(awk 'NR == 1 {print $1; exit}' $FILE)
  if [ "$first" = "//-*-c++-*-" ]; then
    second=$(awk 'NR == 2 {print $1; exit}' $FILE)
    if [ "$second" = "/**" ]; then
      #remove from //-*-c++ to */
      sed '/\/\/-\*-c/,/\*\//d' $FILE >> tmpfile
    else
      #remove  //-*-c++-*-
      sed '/\/\/-\*-c.*/d' $FILE >> tmpfile
    fi
  else
    cat $FILE >> tmpfile
  fi
  # replace file
  mv tmpfile $FILE
done
