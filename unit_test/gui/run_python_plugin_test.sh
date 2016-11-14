TULIP_PYTHON_LIB_DIR=$(dirname $(which tulip_perspective))/../lib/tulip/python
cp python_plugin.py $TULIP_PYTHON_LIB_DIR
sh tulip_run_test.sh python_plugin grid_approximation.tlp grid_approximation.tlp
rm $TULIP_PYTHON_LIB_DIR/python_plugin.py*
