INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TulipCoreInclude} ${TulipCoreBuildInclude} ${OGDFInclude} ${TulipOGDFInclude})

SET(PLUGINS_SRCS
OGDFFm3.cpp
OGDFUpwardPlanarization.cpp
OGDFFrutchermanReingold.cpp
OGDFFastMultipoleMultilevelEmbedder.cpp
OGDFFastMultipoleEmbedder.cpp
OGDFVisibility.cpp
OGDFKamadaKawai.cpp
OGDFCircular.cpp
OGDFBalloon.cpp
OGDFDavidsonHarel.cpp
OGDFMMMExampleNoTwistLayout.cpp
OGDFTree.cpp
OGDFMMMExampleFastLayout.cpp
OGDFGemFrick.cpp
OGDFStressMajorization.cpp
OGDFSugiyama.cpp
OGDFDominance.cpp
OGDFMMMExampleNiceLayout.cpp
OGDFPlanarizationGrid.cpp
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOGDF_DLL")

DISABLE_COMPILER_WARNINGS()

FOREACH(OGDFPlugin ${PLUGINS_SRCS})
  STRING(REGEX REPLACE ".cpp" "" PLUGIN_NAME ${OGDFPlugin})
  STRING(REGEX REPLACE ".*/" "" PLUGIN_NAME ${PLUGIN_NAME})
  STRING(TOLOWER ${PLUGIN_NAME} PLUGIN_NAME)
  ADD_LIBRARY(${PLUGIN_NAME}-${TulipVersion} SHARED ${OGDFPlugin})
  TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TulipVersion} ${TulipLibName} ${LibTulipOGDFName} ${OGDF_LIBRARY})

  INSTALL(TARGETS ${PLUGIN_NAME}-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT tulip_ogdf_plugins)
ENDFOREACH()
