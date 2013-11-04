. ./init_dirs.sh

echo "compiling tulip"

CLUSTERING_FILES=$(find ${TLP_SRC_DIR}/plugins/clustering -name '*.cpp' -a ! -name 'Convolution*.cpp')
EDGE_BUNDLING_FILES=$(find ${TLP_SRC_DIR}/plugins/general/EdgeBundling -name '*.cpp')
LAYOUT_FILES=$(find ${TLP_SRC_DIR}/plugins/layout -name '*.cpp' -a ! -name Dendrogram.cpp -a ! -name ImprovedWalker.cpp -a ! -name TreeLeaf.cpp)
OGDF_FILES=$(find ${TLP_SRC_DIR} -path '*library/tulip-ogdf*' -a -name '*.cpp' -o -path '*thirdparty/OGDF/src*' -a -name '*.cpp')
OGDF_INCLUDES=-I${TLP_SRC_DIR}/thirdparty/OGDF
METRIC_FILES=$(find ${TLP_SRC_DIR}/plugins/metric -name '*.cpp')
SELECTION_FILES=$(find ${TLP_SRC_DIR}/plugins/selection -name '*.cpp')
SIZES_FILES=$(find ${TLP_SRC_DIR}/plugins/sizes -name '*.cpp')
TEST_FILES=$(find ${TLP_SRC_DIR}/plugins/test -name '*.cpp')

em++ -s ALLOW_MEMORY_GROWTH=1 -std=c++0x -DNDEBUG -I${TLP_SRC_DIR}/thirdparty -I${TLP_SRC_DIR}/thirdparty/gzstream -I${TLP_SRC_DIR}/thirdparty/yajl/src ${OGDF_INCLUDES} -I${TLP_SRC_DIR}/library/tulip-core/include/ -I${TLP_INSTALL_DIR}/include/ ${TLP_SRC_DIR}/library/tulip-core/src/*.cpp ${OGDF_FILES} ${METRIC_FILES} ${SELECTION_FILES} ${SIZES_FILES} ${TEST_FILES} ${LAYOUT_FILES} ${CLUSTERING_FILES} ${EDGE_BUNDLING_FILES} -o libtulip.o

if [ $? -eq 0 ]; then
  echo "libtulip.o generated"
fi
