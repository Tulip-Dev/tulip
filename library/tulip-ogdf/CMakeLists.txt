INCLUDE_DIRECTORIES(${OGDFInclude} ${TulipInclude} ${TulipBuildInclude})

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
