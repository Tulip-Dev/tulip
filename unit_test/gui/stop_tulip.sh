# stop tulip if it is still running
if [ -f tulip.pid ]; then
    TULIP_PID=`cat tulip.pid`
    ps -p $TULIP_PID -no-headers > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        # ensure tlp file is saved
	sleep 4
	echo "Tulip stopped"
	kill $TULIP_PID
    fi
fi
