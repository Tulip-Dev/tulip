. ./init_dirs.sh

echo "compiling gzstream"

em++ -s ALLOW_MEMORY_GROWTH=1 -std=c++0x -O2 -DNDEBUG -I${TLP_SRC_DIR}/thirdparty/gzstream/ ${TLP_SRC_DIR}/thirdparty/gzstream/*.cpp  -o gzstream.o

if [ $? -eq 0 ]; then
  echo "gzstream.o generated"
fi

