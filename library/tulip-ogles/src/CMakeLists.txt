SET(tulip-ogles_LIB_SRCS
  Camera.cpp
  GlBoundingBoxSceneVisitor.cpp
  GlBuffer.cpp
  GlComposite.cpp
  GlConcavePolygon.cpp
  GlConvexPolygon.cpp
  GlCPULODCalculator.cpp
  GlEntity.cpp
  GlFrameBufferObject.cpp
  GlGlyphEntity.cpp
  GlGraph.cpp
  GlGraphInputData.cpp
  GlGraphStaticData.cpp
  GlGraphRenderingParameters.cpp
  GlGrid.cpp
  GlLayer.cpp
  GlLODSceneVisitor.cpp
  GlQuadTreeLODCalculator.cpp
  GlRect2D.cpp
  GlScene.cpp
  GlShaderProgram.cpp
  GlUtils.cpp
  LabelsRenderer.cpp
  Light.cpp
  NanoVGManager.cpp
  ShaderManager.cpp
  TextureAtlas.cpp
  GlTextureManager.cpp
  ZoomAndPanAnimation.cpp
  OpenGlConfigManager.cpp
  Glyph.cpp
  GlyphsManager.cpp
  GlyphsRenderer.cpp
  CircleGlyph.cpp
  TriangleGlyph.cpp
  FontIconGlyph.cpp
  GlLine.cpp
  ConcaveHullBuilder.cpp
)

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${Tess2Include} ${STBInclude} ${ClipperInclude})
INCLUDE_DIRECTORIES(${FTGLInclude} ${NanoVGInclude} ${FREETYPE_INCLUDE_DIRS} ${UTF8CppInclude})
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLESInclude} )


ADD_LIBRARY (${LibTulipOGLESName} SHARED ${tulip-ogles_LIB_SRCS})
TARGET_LINK_LIBRARIES(${LibTulipOGLESName} ${LibTulipCoreName} ${FTGLLibrary} ${GLEW_LIBRARY}
                       ${OPENGL_gl_LIBRARY} ${FREETYPE_LIBRARY} ${Tess2Library} ${ClipperLibrary} nanovg)

INSTALL(TARGETS ${LibTulipOGLESName}
  RUNTIME DESTINATION ${TulipBinInstallDir}
  LIBRARY DESTINATION ${TulipLibInstallDir}
  ARCHIVE DESTINATION ${TulipLibInstallDir})

IF(ACTIVATE_PYTHON_WHEELS_TARGETS)
COPY_TARGET_LIBRARY_POST_BUILD(${LibTulipOGLESName} ${TULIPOGL_PYTHON_NATIVE_FOLDER} wheels)
IF(LINUX)
COPY_TARGET_LIBRARY_POST_BUILD(${LibTulipOGLESName} ${TULIPGUI_PYTHON_NATIVE_FOLDER} wheels)
ENDIF(LINUX)
ENDIF(ACTIVATE_PYTHON_WHEELS_TARGETS)
