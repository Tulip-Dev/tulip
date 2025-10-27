echo "Launching Tulip ..."
if [ -f tulip.pid ]; then
  rm tulip.pid
fi

TULIP=$1

if [ "$1" = "" ]; then
  TULIP=tulip
fi

#$TULIP $2 --perspective=Tulip --gui_testing=1 --geometry=100,73,1600,900 -style plastique &
$TULIP --perspective=Tulip --gui_testing --gui_geometry 100,73,1600,900 -style plastique $2 &
TULIP_PID=$!
echo $TULIP_PID > tulip.pid
sleep 3
# check for crash at launch time
ps --pid $TULIP_PID
if [ $? -eq 1 ]; then
    rm tulip.pid
    exit
fi

wait $TULIP_PID
CNEE_PID=`ps -eo pid,comm | grep cnee | awk '{if (NR == 1) print $1}'`
if [ "$CNEE_PID" != "" ]; then
  kill -9 $CNEE_PID
fi
