INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLESInclude} ${CMAKE_CURRENT_SOURCE_DIR}/src/interactors)
INCLUDE_DIRECTORIES(${UTF8CppInclude} ${ClipperInclude} ${STBInclude})
INCLUDE_DIRECTORIES(${Tess2Include} ${FTGLInclude} ${NanoVGInclude})

SET(USE_DFE OFF CACHE BOOL "Use duplicate functions elimination to reduce emitted js file size (time consuming)")
SET(USE_WASM OFF CACHE BOOL "Use WebAssembly (experimental) instead of asm.js")
SET(LIGHTWEIGHT_TULIPJS OFF CACHE BOOL "Compile a lightweight version of tulip.js : contains only the graph visualization features, no algorithms bundled and no Worker created")

SET(TULIPJS_SRCS
  src/TulipHtml5.cpp
  src/TulipBindings.cpp
  src/main.cpp
  src/TimeMeasurer.cpp
  src/interactors/FisheyeInteractor.cpp
  src/interactors/LassoSelectionInteractor.cpp
  src/interactors/NeighborhoodInteractor.cpp
  src/interactors/RectangleZoomInteractor.cpp
  src/interactors/SelectionInteractor.cpp
  src/interactors/SelectionModifierInteractor.cpp
  src/interactors/ZoomAndPanInteractor.cpp
)

SET(CMAKE_EXECUTABLE_SUFFIX "")

ADD_EXECUTABLE(tulip.js ${TULIPJS_SRCS})

TARGET_LINK_LIBRARIES(tulip.js ${LibTulipCoreName} ${LibTulipOGLESName} ${FTGLLibrary} ${Tess2Library} ${ClipperLibrary} ${NanoVGLibrary})
SET(USE_WORKER OFF)

SET(TULIP_PLUGINS_LIBRARIES Delaunay-${TulipVersion}
                            Voronoi-${TulipVersion}
                            StrengthClustering-${TulipVersion}
                            HierarchicalClustering-${TulipVersion}
                            EqualValueClustering-${TulipVersion}
                            QuotientClustering-${TulipVersion}
                            GMLExport-${TulipVersion}
                            CurveEdges-${TulipVersion}
                            ReverseEdges-${TulipVersion}
                            EdgeBundling-${TulipVersion}
                            PlanarGraph-${TulipVersion}
                            DotImport-${TulipVersion}
                            Grid-${TulipVersion}
                            GMLImport-${TulipVersion}
                            RandomGraph-${TulipVersion}
                            RandomSimpleGraph-${TulipVersion}
                            AdjacencyMatrixImport-${TulipVersion}
                            CompleteGraph-${TulipVersion}
                            RandomTree-${TulipVersion}
                            RandomTreeGeneral-${TulipVersion}
                            CompleteTree-${TulipVersion}
                            SmallWorldGraph-${TulipVersion}
                            ImportPajek-${TulipVersion}
                            ImportUCINET-${TulipVersion}
                            EmptyGraph-${TulipVersion}
                            OGDFFm3-${TulipVersion}
                            OGDFUpwardPlanarization-${TulipVersion}
                            OGDFFrutchermanReingold-${TulipVersion}
                            OGDFFastMultipoleMultilevelEmbedder-${TulipVersion}
                            OGDFFastMultipoleEmbedder-${TulipVersion}
                            OGDFVisibility-${TulipVersion}
                            OGDFKamadaKawai-${TulipVersion}
                            OGDFCircular-${TulipVersion}
                            OGDFBalloon-${TulipVersion}
                            OGDFDavidsonHarel-${TulipVersion}
                            OGDFMMMExampleNoTwistLayout-${TulipVersion}
                            OGDFTree-${TulipVersion}
                            OGDFMMMExampleFastLayout-${TulipVersion}
                            OGDFGemFrick-${TulipVersion}
                            OGDFStressMajorization-${TulipVersion}
                            OGDFSugiyama-${TulipVersion}
                            OGDFDominance-${TulipVersion}
                            OGDFMMMExampleNiceLayout-${TulipVersion}
                            OGDFPlanarizationGrid-${TulipVersion}
                            OGDFBertaultLayout-${TulipVersion}
                            OGDFPivotMDS-${TulipVersion}
                            OGDFTileToRowsPacking-${TulipVersion}
                            OGDFPlanarizationLayout-${TulipVersion}
                            FastOverlapRemoval-${TulipVersion}
                            Grip-${TulipVersion}
                            LinLog-${TulipVersion}
                            MixedModel-${TulipVersion}
                            ConnectedComponentPacking-${TulipVersion}
                            Random-${TulipVersion}
                            GemLayout-${TulipVersion}
                            TreeReingoldAndTilforExtended-${TulipVersion}
                            ConeTreeExtended-${TulipVersion}
                            TreeRadial-${TulipVersion}
                            TreeLeaf-${TulipVersion}
                            BubbleTree-${TulipVersion}
                            Circular-${TulipVersion}
                            HierarchicalGraph-${TulipVersion}
                            Tutte-${TulipVersion}
                            Dendrogram-${TulipVersion}
                            ImprovedWalker-${TulipVersion}
                            SquarifiedTreeMap-${TulipVersion}
                            PerfectLayout-${TulipVersion}
                            PolyominoPacking-${TulipVersion}
                            Eccentricity-${TulipVersion}
                            DegreeMetric-${TulipVersion}
                            ClusterMetric-${TulipVersion}
                            StrengthMetric-${TulipVersion}
                            BiconnectedComponent-${TulipVersion}
                            ConnectedComponent-${TulipVersion}
                            StrongComponent-${TulipVersion}
                            DagLevelMetric-${TulipVersion}
                            IdMetric-${TulipVersion}
                            LeafMetric-${TulipVersion}
                            NodeMetric-${TulipVersion}
                            DepthMetric-${TulipVersion}
                            PathLengthMetric-${TulipVersion}
                            StrahlerMetric-${TulipVersion}
                            RandomMetric-${TulipVersion}
                            BetweennessCentrality-${TulipVersion}
                            KCores-${TulipVersion}
                            LouvainClustering-${TulipVersion}
                            LinkCommunities-${TulipVersion}
                            MCLClustering-${TulipVersion}
                            PageRank-${TulipVersion}
                            WelshPowell-${TulipVersion}
                            SpanningDagSelection-${TulipVersion}
                            ReachableSubGraphSelection-${TulipVersion}
                            SpanningTreeSelection-${TulipVersion}
                            InducedSubGraphSelection-${TulipVersion}
                            LoopSelection-${TulipVersion}
                            MultipleSelection-${TulipVersion}
                            Kruskal-${TulipVersion}
                            MetricSizeMapping-${TulipVersion}
                            AutoSize-${TulipVersion}
                            Planarity-${TulipVersion}
                            Simple-${TulipVersion}
                            Tree-${TulipVersion}
                            Connected-${TulipVersion}
                            Biconnected-${TulipVersion}
                            Triconnected-${TulipVersion}
                            Outerplanar-${TulipVersion}
                            Acyclic-${TulipVersion}
                            ColorMapping-${TulipVersion})

IF(NOT LIGHTWEIGHT_TULIPJS)
  TARGET_LINK_LIBRARIES(tulip.js ${LibTulipOGDFName} ${TULIP_PLUGINS_LIBRARIES})
  SET(USE_WORKER ON)
ENDIF(NOT LIGHTWEIGHT_TULIPJS)

SET(TULIP_JS_LINK_FLAGS "--bind -s USE_ZLIB=1 -s USE_FREETYPE=1 -s LZ4=1 -s TOTAL_MEMORY=268435456 -s MODULARIZE=1 -s EVAL_CTORS=0 -s EXPORT_NAME=\\\"tulipjs\\\" --preload-file resources/")

IF(USE_DFE AND NOT USE_WASM)
SET(TULIP_JS_LINK_FLAGS "${TULIP_JS_LINK_FLAGS} -s ELIMINATE_DUPLICATE_FUNCTIONS=1")
ENDIF(USE_DFE AND NOT USE_WASM)

IF(USE_WASM)

SET(TULIP_JS_LINK_FLAGS "${TULIP_JS_LINK_FLAGS} -s BINARYEN=1 -s 'BINARYEN_METHOD=\"native-wasm\"' -s 'BINARYEN_SCRIPTS=\"spidermonkify.py\"' -s GLOBAL_BASE=1000 -s ALIASING_FUNCTION_POINTERS=0")

ENDIF(USE_WASM)

SET_TARGET_PROPERTIES(tulip.js PROPERTIES LINK_FLAGS ${TULIP_JS_LINK_FLAGS})

EM_LINK_PRE_JS(tulip.js ${CMAKE_CURRENT_SOURCE_DIR}/src/tulip_pre.js)
EM_LINK_POST_JS(tulip.js ${CMAKE_CURRENT_SOURCE_DIR}/src/tulip_post.js)
EM_LINK_JS_LIBRARY(tulip.js ${CMAKE_CURRENT_SOURCE_DIR}/src/tulip_library.js)

ADD_CUSTOM_COMMAND(TARGET tulip.js POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -DUSE_WASM=${USE_WASM} -DTULIP_VIZ_FEATURES=ON -DTULIP_USE_WORKER=${USE_WORKER} -DJSFILE=tulip.js -DJSUTILSFILE=${CMAKE_CURRENT_SOURCE_DIR}/src/utils.js -P ${CMAKE_CURRENT_SOURCE_DIR}/src/wrap_compiled_code.cmake
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

ADD_CUSTOM_TARGET(copy-tulipjs-resources ALL
                 COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/resources
                 COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/library/tulip-ogles/bitmaps/font.ttf ${CMAKE_CURRENT_BINARY_DIR}/resources
                 COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/library/tulip-ogles/bitmaps/fontawesome-webfont.ttf ${CMAKE_CURRENT_BINARY_DIR}/resources
                 COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/library/tulip-ogles/bitmaps/materialdesignicons-webfont.ttf ${CMAKE_CURRENT_BINARY_DIR}/resources
                 COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/library/tulip-ogles/bitmaps/cylinderTexture.png ${CMAKE_CURRENT_BINARY_DIR}/resources
                 COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/textures/radialGradientTexture.png ${CMAKE_CURRENT_BINARY_DIR}/resources)
ADD_DEPENDENCIES(tulip.js copy-tulipjs-resources)

IF(NOT CMAKE_DEBUG_MODE)

ADD_CUSTOM_TARGET(copy-tulipjs-to-demos ALL
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/tulip.js ${CMAKE_CURRENT_SOURCE_DIR}/demos
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/tulip.js.mem ${CMAKE_CURRENT_SOURCE_DIR}/demos
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/tulip.data ${CMAKE_CURRENT_SOURCE_DIR}/demos)
INSTALL(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/tulip.js
  ${CMAKE_CURRENT_BINARY_DIR}/tulip.js.mem
  ${CMAKE_CURRENT_BINARY_DIR}/tulip.data
  demos/tulip_web.html
  demos/spinner.css
  DESTINATION ${CMAKE_INSTALL_PREFIX}
)

ELSE(NOT CMAKE_DEBUG_MODE)

ADD_CUSTOM_TARGET(copy-tulipjs-to-demos ALL
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/tulip.js ${CMAKE_CURRENT_SOURCE_DIR}/demos
                  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/tulip.data ${CMAKE_CURRENT_SOURCE_DIR}/demos)
INSTALL(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/tulip.js
  ${CMAKE_CURRENT_BINARY_DIR}/tulip.data
  demos/tulip_web.html
  demos/spinner.css
  DESTINATION ${CMAKE_INSTALL_PREFIX}
)

ENDIF(NOT CMAKE_DEBUG_MODE)

ADD_DEPENDENCIES(copy-tulipjs-to-demos tulip.js)

INSTALL(DIRECTORY
  demos/data
  demos/images
  DESTINATION ${CMAKE_INSTALL_PREFIX}
)
