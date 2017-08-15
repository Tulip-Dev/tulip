ADD_SUBDIRECTORY(antlr)
ADD_SUBDIRECTORY(xdkbibtex)

INCLUDE_DIRECTORIES(./ ${TulipCoreBuildInclude} ${TulipCoreInclude})        

FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(BibTeXImport-${TulipVersion} SHARED ${LIB_SRCS} ${RCC_SRCS})
TARGET_LINK_LIBRARIES(BibTeXImport-${TulipVersion} ${LibTulipCoreName} xdkbibtex)

TULIP_INSTALL_PLUGIN(BibTeXImport-${TulipVersion} ${TulipPluginsInstallDir})
