FILES=$(find . -name CVS -prune -o -name "*.h" -print -o -name "*.cpp" -print -o -name "*.cxx" -print)

for FILE in $FILES
do
  echo "new header for $FILE"
  # normalize header
  sed '1s/\/\/-\*-C.*/\/\/-\*-c\+\+-\*-/' $FILE > tmpfile
  mv tmpfile $FILE
  # output header
  echo '//-*-c++-*-' > tmpfile
  pattern=$(echo 's/dd\/mm\/yyyy/'`date '+%d\/%m\/%Y/'`)
  sed $pattern ./header.txt >> tmpfile
  first=$(awk 'NR == 1 {print $1; exit}' $FILE)
  if [ "$first" = "//-*-c++-*-" ]; then
    second=$(awk 'NR == 2 {print $1; exit}' $FILE)
    if [ "$second" = "/**" -o "$second" = "/*" ]; then
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
