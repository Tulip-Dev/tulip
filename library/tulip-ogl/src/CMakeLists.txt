SET(tulip-ogl_LIB_SRCS
  AbstractGlCurve.cpp
  AroundTexturedSphere.cpp
  Camera.cpp
  CubeOutLined.cpp
  Curves.cpp
  edgeExtremityArrow.cpp
  EdgeExtremityGlyph.cpp
  EdgeExtremityGlyphManager.cpp
  Gl2DRect.cpp
  GlAbstractPolygon.cpp
  GlAxis.cpp
  GlBezierCurve.cpp
  GlBoundingBoxSceneVisitor.cpp
  GlBox.cpp
  GlCatmullRomCurve.cpp
  GlCircle.cpp
  GlColorScale.cpp
  GlComplexPolygon.cpp
  GlComposite.cpp
  GlConvexGraphHull.cpp
  GlConvexHull.cpp
  GlCPULODCalculator.cpp
  GlCubicBSplineInterpolation.cpp
  GlCurve.cpp
  GlEdge.cpp
  GlGlyphRenderer.cpp
  GlGraphComposite.cpp
  GlGraphRenderer.cpp
  GlGraphHighDetailsRenderer.cpp
  GlGraphLowDetailsRenderer.cpp
  GlGraphInputData.cpp
  GlGraphRenderingParameters.cpp
  GlGraphStaticData.cpp
  GlGrid.cpp
  GlHexagon.cpp
  GlLabel.cpp
  GlLayer.cpp
  GlLine.cpp
  GlLines.cpp
  GlMetaNodeRenderer.cpp
  GlNode.cpp
  GlNominativeAxis.cpp
  GlOpenUniformCubicBSpline.cpp
  GlPentagon.cpp
  GlPolygon.cpp
  GlPolyQuad.cpp
  GlProgressBar.cpp
  GlQuad.cpp
  GlQuadTreeLODCalculator.cpp
  GlQuantitativeAxis.cpp
  GlRect.cpp
  GlRegularPolygon.cpp
  GlScene.cpp
  GlSceneObserver.cpp
  GlSceneZoomAndPan.cpp
  GlSelectSceneVisitor.cpp
  GlShaderProgram.cpp
  GlSimpleEntity.cpp
  GlSphere.cpp
  GlStar.cpp
  GlTextureManager.cpp
  GlTools.cpp
  GlTriangle.cpp
  GlVertexArrayManager.cpp
  GlXMLTools.cpp
  Glyph.cpp
  GlyphManager.cpp
  OpenGlConfigManager.cpp
  TulipFontAwesome.cpp
  TulipIconicFont.cpp
  TulipMaterialDesignIcons.cpp
)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${PROJECT_SOURCE_DIR} ${FTGLInclude} ${FREETYPE_INCLUDE_DIRS} ${UTF8CppInclude})
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${Tess2Include})
IF(TULIP_BUILD_GL_TEX_LOADER)
  INCLUDE_DIRECTORIES(${JPEG_INCLUDE_DIR} ${PNG_INCLUDE_DIR})
ENDIF(TULIP_BUILD_GL_TEX_LOADER)

ADD_LIBRARY (${LibTulipOGLName} SHARED ${tulip-ogl_LIB_SRCS})
TARGET_LINK_LIBRARIES(${LibTulipOGLName} ${FTGLLibrary} ${GLEW_LIBRARY}
                      ${LibTulipCoreName} ${OPENGL_gl_LIBRARY}
                      ${FREETYPE_LIBRARY})
IF(TULIP_BUILD_GL_TEX_LOADER)
  TARGET_LINK_LIBRARIES(${LibTulipOGLName} ${JPEG_LIBRARY} ${PNG_LIBRARY})
ENDIF(TULIP_BUILD_GL_TEX_LOADER)

INSTALL(TARGETS ${LibTulipOGLName}
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_ogl
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_ogl
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_ogl)
