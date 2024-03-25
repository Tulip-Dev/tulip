# copy a simple python plugin in the current user python plugins home dir
TULIP_VERSION=$($(dirname $(which tulip))/tulip-config --version | awk -F. '{printf("%d.%d", $1, $2)}')
TULIP_PYTHON_PLUGINS_DIR=$HOME/.Tulip-${TULIP_VERSION}/plugins/python
mkdir -p ${TULIP_PYTHON_PLUGINS_DIR}
cp python_plugin.py ${TULIP_PYTHON_PLUGINS_DIR}
# lauch test
sh tulip_run_test.sh python_plugin grid_approximation.tlp grid_approximation.tlp
# remove python plugin
rm ${TULIP_PYTHON_PLUGINS_DIR}/python_plugin.py*
# remove python cache dir to avoid to disturb following gui tests
rm -rf *pycache*
