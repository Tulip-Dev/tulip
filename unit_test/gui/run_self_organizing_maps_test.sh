# backup original Tulip settings
cp ~/.config/TulipSoftware/Tulip.conf /tmp
sh tulip_run_test.sh self_organizing_maps 
# restore original Tulip settings to ensure that a failure
# of this test will not impact the following ones
cp /tmp/Tulip.conf ~/.config/TulipSoftware
