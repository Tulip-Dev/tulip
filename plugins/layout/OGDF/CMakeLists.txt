INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${TulipCoreBuildInclude} ${TulipCoreInclude} ${OGDFInclude} ${OGDFBuildInclude})

SET(PLUGINS_SRCS
tulip2ogdf/OGDFLayoutPluginBase.cpp
tulip2ogdf/TulipToOGDF.cpp
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
OGDFRadialTree.cpp
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

TULIP_DISABLE_COMPILER_WARNINGS()

ADD_LIBRARY(OGDFPlugins-${Tulip_VERSION} SHARED ${PLUGINS_SRCS})
TARGET_LINK_LIBRARIES(OGDFPlugins-${Tulip_VERSION} ${LibTulipCoreName} ${OGDFLibrary})

TULIP_INSTALL_PLUGIN(OGDFPlugins-${Tulip_VERSION} ${TulipPluginsInstallDir})
