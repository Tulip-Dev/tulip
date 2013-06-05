. ./init_dirs.sh

echo "compiling tulip"

em++ -s ALLOW_MEMORY_GROWTH=1 -std=c++0x -DNDEBUG -I${TLP_SRC_DIR}/thirdparty -I${TLP_SRC_DIR}/thirdparty/gzstream -I${TLP_SRC_DIR}/thirdparty/yajl/src -I${TLP_SRC_DIR}/thirdparty/OGDF -I${TLP_SRC_DIR}/library/tulip-core/include/ -I${TLP_INSTALL_DIR}/include/ ${TLP_SRC_DIR}/library/tulip-core/src/*.cpp ${TLP_SRC_DIR}/library/tulip-ogdf/*.cpp ${TLP_SRC_DIR}/plugins/metric/*.cpp ${TLP_SRC_DIR}/plugins/selection/*.cpp ${TLP_SRC_DIR}/plugins/sizes/*.cpp ${TLP_SRC_DIR}/plugins/test/*.cpp ${TLP_SRC_DIR}/thirdparty/OGDF/src/*/*.cpp ${TLP_SRC_DIR}/plugins/layout/GEMLayout.cpp ${TLP_SRC_DIR}/plugins/layout/Circular.cpp ${TLP_SRC_DIR}/plugins/layout/Rand*.cpp ${TLP_SRC_DIR}/plugins/layout/DatasetTools.cpp ${TLP_SRC_DIR}/plugins/layout/*/*.cpp ${TLP_SRC_DIR}/plugins/layout/Orien*cpp ${TLP_SRC_DIR}/plugins/layout/Bubb*.cpp ${TLP_SRC_DIR}/plugins/layout/Num*cpp ${TLP_SRC_DIR}/plugins/clustering/Delaunay/*.cpp ${TLP_SRC_DIR}/plugins/clustering/EqualValueClustering.cpp ${TLP_SRC_DIR}/plugins/clustering/QuotientClustering.cpp ${TLP_SRC_DIR}/plugins/general/EdgeBundling/*.cpp -o libtulip.o

if [ $? -eq 0 ]; then
  echo "libtulip.o generated"
fi

