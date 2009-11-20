#!/bin/sh
# This script simply replays a scenario of interaction with the tulip gui.
# The first argument is the name cnee recorded events file
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
    echo "usage: $0 <cnee_input_file> [<tulip_input_file>]"
    MUST_EXIT=1
fi

if [ $MUST_EXIT -eq 1 ]; then
    exit
fi

CNEE_INPUT_FILE=$1
TLP_INPUT_FILE=$2

# launch tulip gui
.  ./launch_tulip.sh

# replay events
echo "Replaying events from $CNEE_INPUT_FILE"
$CNEE -rep --file $CNEE_INPUT_FILE -fp > /dev/null 2>&1
echo "Replay finished"

# stop tulip if it is still running
. ./stop_tulip.sh
