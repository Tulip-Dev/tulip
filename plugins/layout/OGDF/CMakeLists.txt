INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TulipCoreBuildInclude} ${TulipCoreInclude} ${OGDFInclude} ${TulipOGDFInclude} ${OGDFBuildInclude})

SET(PLUGINS_SRCS
OGDFFm3.cpp
OGDFUpwardPlanarization.cpp
OGDFFruchtermanReingold.cpp
OGDFFastMultipoleMultilevelEmbedder.cpp
OGDFFastMultipoleEmbedder.cpp
OGDFVisibility.cpp
OGDFKamadaKawai.cpp
OGDFCircular.cpp
OGDFBalloon.cpp
OGDFDavidsonHarel.cpp
OGDFTree.cpp
OGDFGemFrick.cpp
OGDFStressMinimization.cpp
OGDFSugiyama.cpp
OGDFDominance.cpp
OGDFPlanarizationGrid.cpp
OGDFBertaultLayout.cpp
OGDFPivotMDS.cpp
OGDFTileToRowsPacking.cpp
OGDFPlanarizationLayout.cpp
OGDFNodeRespecter.cpp
)

DISABLE_COMPILER_WARNINGS()

FOREACH(OGDFPlugin ${PLUGINS_SRCS})
  STRING(REGEX REPLACE ".cpp" "" PLUGIN_NAME ${OGDFPlugin})
  STRING(REGEX REPLACE ".*/" "" PLUGIN_NAME ${PLUGIN_NAME})
  ADD_LIBRARY(${PLUGIN_NAME}-${TulipVersion} SHARED ${OGDFPlugin})
  TARGET_LINK_LIBRARIES(${PLUGIN_NAME}-${TulipVersion} ${TulipLibName} ${LibTulipOGDFName} ${OGDF_LIBRARY})

  TULIP_INSTALL_PLUGIN(${PLUGIN_NAME}-${TulipVersion} ${TulipPluginsInstallDir})
ENDFOREACH()
