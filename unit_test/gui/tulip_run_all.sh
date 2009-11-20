#!/bin/sh

# This script allows the run of all previously recorded tests

TESTS=

if [ -f all_tests_run.log ]; then
    rm all_tests_run.log
fi

touch all_tests_run.log
 
# run tests loop
for TEST in `ls run_*_test.sh`
do
    echo "Running $TEST ..."
    sh $TEST >> all_tests_run.log 2>&1
done

# check results
NB_TESTS=`grep "test OK" --binary-files=text all_tests_run.log | wc -l`
echo "$NB_TESTS tests (OK)"

NB_FAILURES=`grep "test failed" --binary-files=text all_tests_run.log | wc -l`
if [ $NB_FAILURES -ne 0 ]; then
    echo
    echo "$NB_FAILURES failures"
    grep "test failed" --binary-files=text all_tests_run.log
fi
