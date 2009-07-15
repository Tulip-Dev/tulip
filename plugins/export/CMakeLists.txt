INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})

SET(GMLExport_SRCS
    GMLExport.cpp
)

ADD_LIBRARY(GMLExport-${TulipVersion} SHARED ${GMLExport_SRCS})
TARGET_LINK_LIBRARIES(GMLExport-${TulipVersion} tulip)
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ GMLExport-${TulipVersion})
