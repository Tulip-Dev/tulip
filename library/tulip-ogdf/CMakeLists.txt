include_directories(${OGDFInclude} ${TulipInclude} ${TulipBuildInclude})

SET(TulipToOGDF_SRCS
  TulipToOGDF.cpp
  OGDFLayoutPluginBase.cpp)

add_library(tulipOGDFBridge SHARED ${TulipToOGDF_SRCS})
target_link_libraries(tulipOGDFBridge ${TulipLibName})
target_link_libraries(tulipOGDFBridge ${OGDF_LIBRARY})
