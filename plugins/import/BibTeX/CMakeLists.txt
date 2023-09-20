INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty ${TulipCoreBuildInclude} ${TulipCoreInclude})

FILE(GLOB LIB_SRCS "*.cpp")

ADD_LIBRARY(BibTeXImport-${Tulip_VERSION} SHARED ${LIB_SRCS} ${RCC_SRCS})
TARGET_LINK_LIBRARIES(BibTeXImport-${Tulip_VERSION} ${LibTulipCoreName} xdkbibtex)

TULIP_INSTALL_PLUGIN(BibTeXImport-${Tulip_VERSION} ${TulipPluginsInstallDir})
