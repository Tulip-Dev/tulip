#!/bin/sh

# This script allows to record a new test
# Giving a <test_name> it will create
# a <test_name>.xns file as the cnee events file
# The test must locally produces a tulip result graph
# in the test_gui.tlp file which will be renamed into <test_name>.tlp
# A self sufficient run_<test_name>_test.sh script
# for the test run will also be created

if [ $# -lt 1 ]; then
    echo "$0 failed"
    echo "usage: $0 <test_name> <tlp_input_file>"
    exit 1
fi

TEST_NAME=$1
TLP_INPUT_FILE=$2

# $TLP_INPUT_FILE must exist
if [ "$TLP_INPUT_FILE" != "" ]; then
  if [ ! -f $TLP_INPUT_FILE ]; then
    echo "$0 failed: $TLP_INPUT_FILE does not exist"
    exit 1
  fi
fi

# remove remaining test_gui.* files if any
rm -f test_gui.*

if [ -f data/test_gui.tlp ]; then
    rm data/test_gui.tlp
fi

# record the test
sh tulip_record.sh $TEST_NAME.xns $TLP_INPUT_FILE

# check failure
if [ $? -eq 1 ]; then
    echo "Recording of test $TEST_NAME failed"
    exit 1
fi

# $TEST_NAME.xns must exist
if [ ! -f $TEST_NAME.xns ]; then
    echo "Recording of $TEST_NAME failed: $TEST_NAME.xns does not exist"
    exit 1
fi

# move data/test_gui.tlp if it exist
if [ -f data/test_gui.tlp ]; then
    mv data/test_gui.tlp .
fi

# test_gui.tlp must exist
if [ ! -f test_gui.tlp ]; then
    echo "Recording of $TEST_NAME failed: test_gui.tlp does not exist"
    exit
fi

echo $TEST_NAME.xns created

# rename test_gui.tlp
mv test_gui.tlp $TEST_NAME.tlp

# create script file for test run
echo "sh tulip_run_test.sh $TEST_NAME $TLP_INPUT_FILE" > run_${TEST_NAME}_test.sh
echo run_${TEST_NAME}_test.sh created