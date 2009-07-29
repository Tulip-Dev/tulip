if [ $# -lt 2 ]; then
  echo "usage: $(basename $0) plugins_src_top_dir plugins_server_dir"
  exit
fi

TULIP_VERSION=`$(dirname $0)/../../../../tulip-config --version`

SRC_TOP_DIR=$1
SRV_DIR=$2

if [ -d /tmp/plugins_doc ]; then
  rm -rf /tmp/plugins_doc
else
  mkdir /tmp/plugins_doc
fi


LIB_DIRS=`find ${SRC_TOP_DIR} -name "lib*-${TULIP_VERSION}.so" -exec dirname {} ";"`
for LIB_DIR in ${LIB_DIRS}
do
DIR=$(basename $LIB_DIR)
FILE_H=${LIB_DIR}/${DIR}.h
FILE_CPP=${LIB_DIR}/${DIR}.cpp
if [ -f $FILE_H ]; then
FILE=${FILE_H}
else
FILE=${FILE_CPP}
fi
sh generate_plugin_doc.sh ${FILE} /tmp/plugins_doc
./pluginInstaller ${LIB_DIR}/lib*.so /tmp/plugins_doc/xml/${DIR}*.xml $SRV_DIR
done

