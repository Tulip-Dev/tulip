. ./init_dirs.sh

echo "compiling ftgl"

em++ -s ALLOW_MEMORY_GROWTH=1 -std=c++0x -O2 -DNDEBUG -I${TLP_SRC_DIR}/thirdparty/ftgl ${TLP_SRC_DIR}/thirdparty/ftgl/*.cpp  -I/usr/include -I/usr/include/freetype2 -DFT_USE_AUTOCONF_SIZEOF_TYPES -o libftgl.o

if [ $? -eq 0 ]; then
  echo "libftgl.o generated"
fi
