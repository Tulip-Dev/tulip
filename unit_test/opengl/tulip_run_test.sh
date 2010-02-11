#!/bin/sh

# This script allows the run of an already recorded test
# Giving a <test_name> it assumes the existence of
# <test_name>.xns which is the cnee events file
# and if no tlp_output_file is given,
# <test_name>.tlp which is the expected tulip result graph
# If no tlp_input_file is given, tlp_output_file is set
# to empty_graph.tlp.
# The test must locally produces a tulip result graph
# in the test_gui.tlp file
# After the test run, the files <test_name>.tlp and test_gui.tlp
# are compared; the test is successful if they only differ from the date field

if [ $# -lt 1 ]; then
    echo "$0 running test failed"
    echo "usage: $0 <test_name> [<tlp_input_file> tlp_output_file = test_name.tlp]"
    exit
fi

TEST_NAME=$1
TLP_INPUT_FILE=$2
TLP_OUTPUT_FILE=$3

echo "***********************************************"
echo "Running ${TEST_NAME} test"
echo "***********************************************"

# check if $TLP_INPUT_FILE exists
if [ "$TLP_INPUT_FILE" != "" ]; then
  if [ ! -f $TLP_INPUT_FILE ]; then
    echo "$TEST_NAME test failed: $TLP_INPUT_FILE does not exist"
    exit
  fi
else
  TLP_OUTPUT_FILE=empty_graph.tlp
fi

# $TEST_NAME.xns must exist
if [ ! -f $TEST_NAME.xns ]; then
    echo "$TEST_NAME test failed: $TEST_NAME.xns does not exist"
    exit
fi

# $TEST_NAME.tlp must exist
if [ "$TLP_OUTPUT_FILE" = "" ]; then
  if [ ! -f model_$TEST_NAME.jpg ]; then
    echo "$TEST_NAME test failed: model_$TEST_NAME.jpg does not exist"
    exit
  fi
  TLP_OUTPUT_FILE=model_$TEST_NAME.jpg
fi

# remove test_gui.tlp which is always
# the name of the result graph file
if [ -f test_gui.jpg ]; then
    rm test_gui.jpg
fi

# run the test
sh tulip_replay.sh $TEST_NAME.xns $TLP_INPUT_FILE

# test_gui.tlp must exist
if [ ! -f test_gui.jpg ]; then
    echo "$TEST_NAME test failed: test_gui.jpg does not exist"
    cp test_failed.jpg test_gui.jpg
    exit
fi

NB_DIFFS=0
# check the result jpg file
if [ $(diff $TLP_OUTPUT_FILE test_gui.jpg | wc -l) -gt $NB_DIFFS ]; then
    mv test_gui.jpg result_${TEST_NAME}.jpg
    echo "$TEST_NAME test failed: diff failure between ${TLP_OUTPUT_FILE} & model_${TEST_NAME}.jpg"
else
    rm test_gui.jpg
    echo "*** $TEST_NAME test OK ***"
fi