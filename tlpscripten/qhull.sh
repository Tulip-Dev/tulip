. ./init_dirs.sh

echo "compiling qhull"

emcc -s ALLOW_MEMORY_GROWTH=1 -DNDEBUG -I${TLP_SRC_DIR}/thirdparty/libqhull/ ${TLP_SRC_DIR}/thirdparty/libqhull/*.c  -o libqhull.o

if [ $? -eq 0 ]; then
  echo "libqhull.o generated"
fi
