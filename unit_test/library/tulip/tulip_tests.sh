if [ -f tests.log ]; then
rm tests.log
fi

for FILE in $(find -name '*[t|T]est*' -a -type f -a -executable -a ! -name '*.*'
)
do
  echo running tests of $FILE >> tests.log
  $FILE >> tests.log 2>&1
done;

grep ' tests' tests.log
