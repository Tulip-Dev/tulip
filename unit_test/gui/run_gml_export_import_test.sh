# remove test_gui.gml which is the name of the exported gml file
if [ -f test_gui.gml ]; then
    rm test_gui.gml
fi

sh tulip_run_test.sh gml_export_import grid_approximation.tlp grid_approximation.tlp
