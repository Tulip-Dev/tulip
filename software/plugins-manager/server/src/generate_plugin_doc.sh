if [ $# -eq 0 ]; then
  echo "usage: $(basename $0) <input_file> [<output_dir>= .]"
  exit
fi

INPUT_FILE=$1

if [ $# -eq 1 ]; then
  OUPUT_DIR=$2
else
  OUTPUT_DIR = "."
fi

CONFIG_FILE=/tmp/plugin_doc_config.doxygen

cat plugin_doc.doxygen > $CONFIG_FILE
echo "INPUT = $INPUT_FILE" >> $CONFIG_FILE
echo "OUTPUT_DIRECTORY = $OUTPUT_DIR" >> $CONFIG_FILE

doxygen $CONFIG_FILE
