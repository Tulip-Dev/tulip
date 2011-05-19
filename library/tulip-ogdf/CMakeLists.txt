INCLUDE_DIRECTORIES(${OGDFInclude} ${TulipCoreInclude} ${TulipCoreBuildInclude})

IF(WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDLL_TULIP_OGDF -DOGDF_DLL")
ENDIF(WIN32)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

SET(TulipToOGDF_SRCS
  TulipToOGDF.cpp
  OGDFLayoutPluginBase.cpp)

ADD_LIBRARY(${LibTulipOGDFName} SHARED ${TulipToOGDF_SRCS})
TARGET_LINK_LIBRARIES(${LibTulipOGDFName} ${LibTulipCoreName})
TARGET_LINK_LIBRARIES(${LibTulipOGDFName} ${OGDFLibrary})

INSTALL(TARGETS ${LibTulipOGDFName}
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_ogdf
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_ogdf
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_ogdf)

INSTALL(FILES
  tulip2ogdf/OGDFLayoutPluginBase.h
  tulip2ogdf/TulipToOGDF.h
DESTINATION ${TulipIncludeInstallDir}/tulip2ogdf/
COMPONENT tulip_ogdf_dev)
