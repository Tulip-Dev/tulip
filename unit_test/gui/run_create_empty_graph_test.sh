#!/bin/sh

echo "***********************************************"
echo "Running create_empty_graph test"
echo "***********************************************"

# run the test
sh tulip_replay.sh create_empty_graph.xns

# test_gui.tlp must exist
if [ ! -f test_gui.tlp ]; then
    echo "create_empty_graph test failed: test_gui.tlp does not exist"
    exit
fi

# check the result graph file
# the 2 files may only have different dates
if [ $(diff empty_graph.tlp test_gui.tlp | wc -l) -gt 4 ]; then
    mv test_gui.tlp failed_create_empty_graph.tlp
    echo "create_empty_graph test failed: diff failure between empty_graph.tlp & failed_create_empty_graph.tlp"
else
    rm test_gui.tlp
    echo "*** create_empty_graph test OK ***"
fi

