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
  glyphs/CircleGlyph.cpp
  glyphs/ConeGlyph.cpp
  glyphs/CrossGlyph.cpp
  glyphs/CubeGlyph.cpp
  glyphs/CubeOutlinedGlyph.cpp
  glyphs/CubeOutlinedTransparentGlyph.cpp
  glyphs/CylinderGlyph.cpp
  glyphs/DiamondGlyph.cpp
  glyphs/FontAwesomeGlyph.cpp
  glyphs/Glyph.cpp
  glyphs/GlyphsManager.cpp
  glyphs/GlyphsRenderer.cpp
  glyphs/HexagonGlyph.cpp
  glyphs/PentagonGlyph.cpp
  glyphs/RingGlyph.cpp
  glyphs/RoundedBoxGlyph.cpp
  glyphs/SphereGlyph.cpp
  glyphs/SquareGlyph.cpp
  glyphs/StarGlyph.cpp
  glyphs/TriangleGlyph.cpp
)

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${Tess2Include} ${STBInclude})
INCLUDE_DIRECTORIES(${FTGLInclude} ${NanoVGInclude} ${FREETYPE_INCLUDE_DIRS} ${UTF8CppInclude})
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLESInclude} )


ADD_LIBRARY (${LibTulipOGLESName} SHARED ${tulip-ogles_LIB_SRCS})
TARGET_LINK_LIBRARIES(${LibTulipOGLESName} ${LibTulipCoreName} ${FTGLLibrary} ${GLEW_LIBRARY}
                       ${OPENGL_gl_LIBRARY} ${FREETYPE_LIBRARY} ${Tess2Library} nanovg)

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
