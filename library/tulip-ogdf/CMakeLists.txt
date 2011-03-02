INCLUDE_DIRECTORIES(${OGDFInclude} ${TulipInclude} ${TulipBuildInclude})

SET(TulipToOGDF_SRCS
  TulipToOGDF.cpp
  OGDFLayoutPluginBase.cpp)

ADD_LIBRARY(${LibTulipOGDFName} SHARED ${TulipToOGDF_SRCS})
TARGET_LINK_LIBRARIES(${LibTulipOGDFName} ${TulipLibName})
TARGET_LINK_LIBRARIES(${LibTulipOGDFName} ${OGDFLibrary})
INSTALL(TARGETS ${LibTulipOGDFName}
       RUNTIME DESTINATION ${TulipBinInstallDir} 
       LIBRARY DESTINATION ${TulipLibInstallDir} 
       ARCHIVE DESTINATION ${TulipLibInstallDir} )
