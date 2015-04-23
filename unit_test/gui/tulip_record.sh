#!/bin/sh
# This script simply records a scenario of interaction with the tulip gui.
# The first argument is the file in which cnee will record
# the corresponding mouse or keyboard events
# The optional second argument is the tlp file used with tulip

MUST_EXIT=0

CNEE=`which cnee`
if [ $? -eq 1 ]; then
    echo 'cnee executable not found';
    MUST_EXIT=1
fi

TULIP=`which tulip_perspective`
if [ $? -eq 1 ]; then
    echo 'tulip_perspective executable not found';
    MUST_EXIT=1
fi

if [ $# -gt 2 -o $# -lt 1 ]; then
    echo "usage: $0 <cnee_output_file> [<tulip_input_file>]"
    MUST_EXIT=1
fi

if [ $MUST_EXIT -eq 1 ]; then
    exit 1
fi

CNEE_OUTPUT_FILE=$1
TLP_INPUT_FILE=$2

# launch tulip gui
sh  ./launch_tulip.sh $TULIP $TLP_INPUT_FILE > /dev/null 2>&1 &

sleep 5
echo "Recording events in $CNEE_OUTPUT_FILE"
echo "use <End> key to stop recording ..."
$CNEE -rec -etr -1 --keyboard --mouse -sk End -o $CNEE_OUTPUT_FILE
echo "Recording stopped"

# stop tulip if it is still running
. ./stop_tulip.sh
