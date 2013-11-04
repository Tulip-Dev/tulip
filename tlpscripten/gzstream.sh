. ./init_dirs.sh

echo "compiling gzstream"

em++ -s ALLOW_MEMORY_GROWTH=1 -std=c++0x -O2 -DNDEBUG -I${TLP_SRC_DIR}/thirdparty/gzstream/ ${TLP_SRC_DIR}/thirdparty/gzstream/*.cpp  -o libgzstream.o

if [ $? -eq 0 ]; then
  echo "libgzstream.o generated"
fi

