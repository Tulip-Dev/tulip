
MACRO(GEN_PROPERTY_BINDINGS GEN_FILE 
                            BASE_CLASS
                            PROPERTY_TYPE
                            VEC_PROPERTY_TYPE
                            CPP_NODE_TYPE 
                            PYTHON_NODE_TYPE 
                            CPP_EDGE_TYPE 
                            PYTHON_EDGE_TYPE
                            PYTHON_NODE_TYPEHINT
                            PYTHON_EDGE_TYPEHINT
                            PROPERTY_SPECIFIC_METHODS_FILE)

  FILE(READ ${SOURCE_DIR}/AbstractProperty.sip.in input)
  STRING(REPLACE "@BASE_CLASS@" "${BASE_CLASS}" input "${input}")
  STRING(REPLACE "@PROPERTY_TYPE@" "${PROPERTY_TYPE}" input "${input}")
  STRING(REPLACE "@PROPERTY_NODE_TYPE@" "${CPP_NODE_TYPE}" input "${input}")
  STRING(REPLACE "@PROPERTY_EDGE_TYPE@" "${CPP_EDGE_TYPE}" input "${input}")
  STRING(REPLACE "@PYTHON_NODE_TYPE@" "${PYTHON_NODE_TYPE}" input "${input}")
  STRING(REPLACE "@PYTHON_EDGE_TYPE@" "${PYTHON_EDGE_TYPE}" input "${input}")
  STRING(REPLACE "@VEC_PROPERTY_TYPE@" "${VEC_PROPERTY_TYPE}" input "${input}")  
  STRING(REPLACE "@CPP_TYPE@" "${CPP_NODE_TYPE}" input "${input}")
  STRING(REPLACE "@PYTHON_TYPE@" "${PYTHON_NODE_TYPE}" input "${input}")
  STRING(REPLACE "@PYTHON_NODE_TYPEHINT@" "${PYTHON_NODE_TYPEHINT}" input "${input}")
  STRING(REPLACE "@PYTHON_EDGE_TYPEHINT@" "${PYTHON_EDGE_TYPEHINT}" input "${input}")

  IF(NOT "${PROPERTY_SPECIFIC_METHODS_FILE}" STREQUAL "")
    FILE(READ ${SOURCE_DIR}/${PROPERTY_SPECIFIC_METHODS_FILE} specific)
    STRING(REPLACE "@PROPERTY_SPECIFIC_METHODS@" "${specific}" input "${input}")
  ELSE()
    STRING(REPLACE "@PROPERTY_SPECIFIC_METHODS@" "" input "${input}")
  ENDIF()
  
  FILE(WRITE ${BINARY_DIR}/${GEN_FILE} "${input}")
  
ENDMACRO(GEN_PROPERTY_BINDINGS)

GEN_PROPERTY_BINDINGS("BooleanProperty.sip" "PropertyInterface" "Boolean" "Boolean" "bool" "boolean" "bool" "boolean" "bool" "bool" "BooleanPropertySpecific.sip")
GEN_PROPERTY_BINDINGS("DoubleProperty.sip" "NumericProperty" "Double" "Double" "double" "float" "double" "float" "float" "float" "DoublePropertySpecific.sip")
GEN_PROPERTY_BINDINGS("IntegerProperty.sip" "NumericProperty" "Integer" "Integer" "int" "integer" "int" "integer" "int" "int" "IntegerPropertySpecific.sip")
GEN_PROPERTY_BINDINGS("ColorProperty.sip" "PropertyInterface" "Color" "Color" "tlp::Color" ":class:`tlp.Color`" "tlp::Color" ":class:`tlp.Color`" "tlp.Color" "tlp.Color" "")
GEN_PROPERTY_BINDINGS("LayoutProperty.sip" "PropertyInterface" "Layout" "Coord" "tlp::Coord" ":class:`tlp.Coord`" "std::vector<tlp::Coord>" "list of :class:`tlp.Coord`" "tlp.Coord" "List[tlp.Coord]" "LayoutPropertySpecific.sip")
GEN_PROPERTY_BINDINGS("SizeProperty.sip" "PropertyInterface" "Size" "Size" "tlp::Size" ":class:`tlp.Size`" "tlp::Size" ":class:`tlp.Size`" "tlp.Size" "tlp.Size" "SizePropertySpecific.sip")
GEN_PROPERTY_BINDINGS("StringProperty.sip" "PropertyInterface" "String" "String" "std::string" "string" "std::string" "string" "str" "str" "")

   		   
