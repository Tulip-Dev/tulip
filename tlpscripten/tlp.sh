. ./init_dirs.sh

echo "compiling tulip"

em++ -s ALLOW_MEMORY_GROWTH=1 -std=c++0x -DNDEBUG -I${TLP_SRC_DIR}/thirdparty -I${TLP_SRC_DIR}/thirdparty/gzstream -I${TLP_SRC_DIR}/thirdparty/yajl/src -I${TLP_SRC_DIR}/library/tulip-core/include/ -I${TLP_INSTALL_DIR}/include/ -I${TLP_SRC_DIR}/thirdparty/OGDF ${TLP_SRC_DIR}/library/tulip-core/src/*.cpp ${TLP_SRC_DIR}/plugins/metric/*.cpp ${TLP_SRC_DIR}/plugins/selection/*.cpp ${TLP_SRC_DIR}/thirdparty/OGDF/src/*/*.cpp ${TLP_SRC_DIR}/plugins/layout/*.cpp ${TLP_SRC_DIR}/plugins/layout/*/*.cpp ${TLP_SRC_DIR}/plugins/clustering/Delaunay/*.cpp ${TLP_SRC_DIR}/plugins/clustering/EqualValueClustering.cpp ${TLP_SRC_DIR}/plugins/clustering/QuotientClustering.cpp ${TLP_SRC_DIR}/plugins/general/EdgeBundling/*.cpp -o libtulip.o

if [ $? -eq 0 ]; then
  echo "libtulip.o generated"
fi

