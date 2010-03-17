echo "Launching Tulip ..."
if [ -f tulip.pid ]; then
  rm tulip.pid
fi
$1 $2 --x 100 --y 50 --width 1024 --height 900 -style plastique &
TULIP_PID=$!
echo $TULIP_PID > tulip.pid
wait $TULIP_PID
CNEE_PID=`ps -ef | grep cnee | awk '{if (NR == 1) print $2}'`
if [ "$CNEE_PID" != "" ]; then
  kill -9 $CNEE_PID
fi
