#!/bin/sh
# This script simply records a scenario of interaction with the tulip gui.
# The first argument is the name of the file in which cnee will record
# the corresponding events
# The optional second argument is the name of a tlp file used when
# launching Tulip

MUST_EXIT=0

CNEE=`which cnee`
if [ $? -eq 1 ]; then
    echo 'cnee executable not found';
    MUST_EXIT=1
fi

TULIP=`which tulip`
if [ $? -eq 1 ]; then
    echo 'tulip executable not found';
    MUST_EXIT=1
fi

if [ $# -gt 2 -o $# -lt 1 ]; then
    echo "usage: $0 <cnee_output_file> [<tulip_input_file>]"
    MUST_EXIT=1
fi

if [ $MUST_EXIT -eq 1 ]; then
    exit
fi

CNEE_OUTPUT_FILE=$1
TLP_INPUT_FILE=$2

# launch tulip gui
.  ./launch_tulip.sh

echo "Recording events in $CNEE_OUTPUT_FILE"
echo "use <End> key to stop recording ..."
$CNEE -rec -etr -1 --keyboard --mouse -sk End -o $CNEE_OUTPUT_FILE
echo "Recording stopped"

# stop tulip if it is still running
. ./stop_tulip.sh
