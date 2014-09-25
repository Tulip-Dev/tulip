#!/bin/sh
# This script simply records a video
# of the run of a scenario of interactions with the tulip gui.

if [ $# -lt 1 ]; then
    echo "$0 running test failed"
    echo "usage: $0 <test_script>"
    exit
fi

TEST_SCRIPT=$1


if [ ! -f $TEST_SCRIPT ]; then
  echo "$TEST_NAME does not exist"
  exit
fi

# set an evnt variable to ensure recordmydesktop
# will be stopped at the end of the replay
export TLP_RECORDMYDESKTOP=1

# launch the replay
sh $1 &

sleep 3

# check for a crash
if [ ! -f tulip.pid ]; then
  exit
fi

recordmydesktop --windowid $(xwininfo -root -tree | grep "\[Tulip\]" | awk '{print $1}') --no-sound --on-the-fly-encoding -o /tmp/${TEST_SCRIPT} --stop-shortcut Control+s
