#!/bin/sh

# This script allows the run of all previously recorded tests

TYPE=$1
TLP_DIFF_CONTROLLER=$2

# check if diff on controller part in tlp files is excluded (used in tlp_diff.sh)
if [ "$TLP_DIFF_CONTROLLER" = "no_controller_diff" ]; then
  export TLP_DIFF_CONTROLLER=NO
else
  unset TLP_DIFF_CONTROLLER
fi

# clean up log file
if [ -f all_tests_run.log ]; then
    rm all_tests_run.log
fi

touch all_tests_run.log

# launch Tulip for nothing
# just to reduce the future time of shared libs loading
# expecting they will be already in memory
echo "Check Tulip launch ..."
TULIP=`which tulip_perspective`
if [ $? -eq 1 ]; then
    echo 'tulip executable not found';
    exit 1
fi
sh  ./launch_tulip.sh $TULIP > /dev/null 2>&1 &
sleep 10
. ./stop_tulip.sh
 
# run tests loop
TESTS=`ls run_${TYPE}*_test.sh`
NB_TESTS=`echo $TESTS | wc -w`
TEST_ID=0
for TEST in $TESTS
do
    TEST_ID=`expr $TEST_ID + 1`
    echo -n "launching test $TEST_ID/$NB_TESTS: $TEST ..."
    sh $TEST >> all_tests_run.log 2>&1
    # check result
    tail -1 all_tests_run.log | grep OK > /dev/null
    if [ $? -eq 0 ]; then
      echo " OK"
    else
      echo " failed"
    fi
done

# check results
NB_TESTS=`grep "test OK" --binary-files=text all_tests_run.log | wc -l`
echo "$NB_TESTS tests (OK)"

NB_FAILURES=`grep "test failed" --binary-files=text all_tests_run.log | wc -l`
if [ $NB_FAILURES -ne 0 ]; then
    echo
    echo "$NB_FAILURES failures"
    grep "test failed" --binary-files=text all_tests_run.log
    exit 1
fi
