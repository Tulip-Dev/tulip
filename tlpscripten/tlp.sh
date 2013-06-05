. ./init_dirs.sh

echo "compiling tulip"

LAYOUT_FILES=$(find ${TLP_SRC_DIR}/plugins/layout -name '*.cpp' -a ! -name Dendrogram.cpp -a ! -name ImprovedWalker.cpp -a ! -name TreeLeaf.cpp)
CLUSTERING_FILES=$(find ${TLP_SRC_DIR}/plugins/clustering -name '*.cpp' -a ! -name 'Convolution*.cpp')

em++ -s ALLOW_MEMORY_GROWTH=1 -std=c++0x -DNDEBUG -I${TLP_SRC_DIR}/thirdparty -I${TLP_SRC_DIR}/thirdparty/gzstream -I${TLP_SRC_DIR}/thirdparty/yajl/src -I${TLP_SRC_DIR}/thirdparty/OGDF -I${TLP_SRC_DIR}/library/tulip-core/include/ -I${TLP_INSTALL_DIR}/include/ ${TLP_SRC_DIR}/library/tulip-core/src/*.cpp ${TLP_SRC_DIR}/library/tulip-ogdf/*.cpp ${TLP_SRC_DIR}/plugins/metric/*.cpp ${TLP_SRC_DIR}/plugins/selection/*.cpp ${TLP_SRC_DIR}/plugins/sizes/*.cpp ${TLP_SRC_DIR}/plugins/test/*.cpp ${TLP_SRC_DIR}/thirdparty/OGDF/src/*/*.cpp ${LAYOUT_FILES} ${CLUSTERING_FILES} ${TLP_SRC_DIR}/plugins/general/EdgeBundling/*.cpp -o libtulip.o

if [ $? -eq 0 ]; then
  echo "libtulip.o generated"
fi
