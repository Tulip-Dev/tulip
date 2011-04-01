
MACRO(GEN_PROPERTY_BINDINGS TEMPLATE_FILE   
			    GEN_FILE 
			    PROPERTY_TYPE
			    VEC_PROPERTY_TYPE
			    CPP_NODE_TYPE 
			    PYTHON_NODE_TYPE 
			    CPP_EDGE_TYPE 
			    PYTHON_EDGE_TYPE
			    PROPERTY_SPECIFIC_METHODS_FILE)

  FILE(READ ${WORKING_DIR}/${TEMPLATE_FILE} input)
  STRING(REPLACE "@PROPERTY_TYPE@" "${PROPERTY_TYPE}" input "${input}")
  STRING(REPLACE "@PROPERTY_NODE_TYPE@" "${CPP_NODE_TYPE}" input "${input}")
  STRING(REPLACE "@PROPERTY_EDGE_TYPE@" "${CPP_EDGE_TYPE}" input "${input}")
  STRING(REPLACE "@PYTHON_NODE_TYPE@" "${PYTHON_NODE_TYPE}" input "${input}")
  STRING(REPLACE "@PYTHON_EDGE_TYPE@" "${PYTHON_EDGE_TYPE}" input "${input}")
  STRING(REPLACE "@VEC_PROPERTY_TYPE@" "${VEC_PROPERTY_TYPE}" input "${input}")  
  STRING(REPLACE "@CPP_TYPE@" "${CPP_NODE_TYPE}" input "${input}")
  STRING(REPLACE "@PYTHON_TYPE@" "${PYTHON_NODE_TYPE}" input "${input}")

  IF(NOT "${PROPERTY_SPECIFIC_METHODS_FILE}" STREQUAL "")
    FILE(READ ${WORKING_DIR}/${PROPERTY_SPECIFIC_METHODS_FILE} specific)
    STRING(REPLACE "@PROPERTY_SPECIFIC_METHODS@" "${specific}" input "${input}")
  ELSE()
    STRING(REPLACE "@PROPERTY_SPECIFIC_METHODS@" "" input "${input}")
  ENDIF()
  
  FILE(WRITE ${WORKING_DIR}/${GEN_FILE} "${input}")
  
ENDMACRO(GEN_PROPERTY_BINDINGS)

GEN_PROPERTY_BINDINGS("AbstractPropertyBaseType.sip.in" "BooleanProperty.sip" "Boolean" "Boolean" "bool" "bool" "bool" "bool" "BooleanPropertySpecific.sip")
GEN_PROPERTY_BINDINGS("AbstractPropertyBaseType.sip.in" "DoubleProperty.sip" "Double" "Double" "double" "double" "double" "double" "DoublePropertySpecific.sip")
GEN_PROPERTY_BINDINGS("AbstractPropertyBaseType.sip.in" "IntegerProperty.sip" "Integer" "Integer" "int" "int" "int" "int" "IntegerPropertySpecific.sip")
GEN_PROPERTY_BINDINGS("AbstractPropertyPointerType.sip.in" "ColorProperty.sip" "Color" "Color" "tlp::Color" "tlp.Color" "tlp::Color" "tlp.Color" "")
GEN_PROPERTY_BINDINGS("AbstractPropertyPointerType.sip.in" "LayoutProperty.sip" "Layout" "Coord" "tlp::Coord" "tlp.Coord" "std::vector<tlp::Coord>" "list of tlp.Coord" "LayoutPropertySpecific.sip")
GEN_PROPERTY_BINDINGS("AbstractPropertyPointerType.sip.in" "SizeProperty.sip" "Size" "Size" "tlp::Size" "tlp.Size" "tlp::Size" "tlp.Size" "SizePropertySpecific.sip")		   		   
GEN_PROPERTY_BINDINGS("AbstractPropertyPointerType.sip.in" "StringProperty.sip" "String" "String" "std::string" "string" "std::string" "string" "")
   		   