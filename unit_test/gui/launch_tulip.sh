echo "Launching Tulip ..."
if [ -f tulip.pid ]; then
  rm tulip.pid
fi

TULIP=$1

if [ "$1" = "" ]; then
  TULIP=tulip_perspective
fi

$TULIP $2 --perspective=Tulip --geometry=100,50,1600,900 -style plastique &
TULIP_PID=$!
echo $TULIP_PID > tulip.pid
wait $TULIP_PID
CNEE_PID=`ps -eo pid,comm | grep cnee | awk '{if (NR == 1) print $1}'`
if [ "$CNEE_PID" != "" ]; then
  kill -9 $CNEE_PID
fi
