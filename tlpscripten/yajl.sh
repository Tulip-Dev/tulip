. ./init_dirs.sh

echo "compiling yajl"

emcc -s ALLOW_MEMORY_GROWTH=1 -O2 -DNDEBUG -I${TLP_BUILD_DIR}/thirdparty/yajl/yajl-2.0.1/include/ ${TLP_SRC_DIR}/thirdparty/yajl/src/*.c  -o libyajl.o

if [ $? -eq 0 ]; then
  echo "libyajl.o generated"
fi
