echo "Launching Tulip ..."
$TULIP $TLP_INPUT_FILE --x 100 --y 50 --width 1024 --height 900 -style plastique &
TULIP_PID=$!
sleep 4

