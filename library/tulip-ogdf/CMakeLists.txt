INCLUDE_DIRECTORIES(${OGDFInclude} ${TulipInclude} ${TulipBuildInclude})

IF(WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDLL_TULIP_OGDF -DOGDF_DLL")
ENDIF(WIN32)

SET(TulipToOGDF_SRCS
  TulipToOGDF.cpp
  OGDFLayoutPluginBase.cpp)

ADD_LIBRARY(${LibTulipOGDFName} SHARED ${TulipToOGDF_SRCS})
TARGET_LINK_LIBRARIES(${LibTulipOGDFName} ${LibTulipName})
TARGET_LINK_LIBRARIES(${LibTulipOGDFName} ${OGDFLibrary})

INSTALL(TARGETS ${LibTulipOGDFName}
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT libtulip_ogdf
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT libtulip_ogdf
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT libtulip_ogdf)
