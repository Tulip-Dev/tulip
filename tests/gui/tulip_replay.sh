#!/bin/sh
# This script simply replays a scenario of interaction with the tulip gui.
# The mandatory first argument is the cnee recorded events file
# The optional second argument is the tlp file to be used when launching tulip

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
    echo "usage: $0 <cnee_input_file> [<tulip_input_file>]"
    MUST_EXIT=1
fi

if [ $MUST_EXIT -eq 1 ]; then
    exit 1
fi

CNEE_INPUT_FILE=$1
TLP_INPUT_FILE=$2

# launch tulip gui
sh  ./launch_tulip.sh $TULIP $TLP_INPUT_FILE  > /dev/null 2>&1 &

sleep 5
# check for a crash
if [ ! -f tulip.pid ]; then
  exit 1
fi

# replay events
if [ "$TLP_REPLAY_SPEED" != "" ]; then
  SPEED="-sp $TLP_REPLAY_SPEED"
fi

echo "Replaying events from $CNEE_INPUT_FILE"
$CNEE -rep $SPEED --file $CNEE_INPUT_FILE -fcr -fp -e /tmp/$CNEE_INPUT_FILE.log > /dev/null 2>&1
#reset auto repeat
xset r
echo "Replay finished"

# stop recordmydesktop if needed
if [ "$TLP_RECORDMYDESKTOP" = "1" ]; then
# send a TERM signal
  PID=$(ps -ef | grep recordmydesktop | grep -v grep | awk '{print $2}')
  kill -15 $PID
fi

# stop tulip if it is still running
. ./stop_tulip.sh

exit 0
