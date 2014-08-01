SET(tulip-ogl_LIB_SRCS 
  AbstractGlCurve.cpp
  Camera.cpp
  CubeOutLined.cpp
  Curves.cpp
  edgeExtremityArrow.cpp
  EdgeExtremityGlyph.cpp
  EdgeExtremityGlyphManager.cpp
  EpsFunction.cpp
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
  GlDisplayListManager.cpp
  GlEdge.cpp
  GlEPSFeedBackBuilder.cpp
  GlFeedBackRecorder.cpp
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
  GlLODSceneVisitor.cpp
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
  GlSVGFeedBackBuilder.cpp
  GlTextureManager.cpp
  GlTLPFeedBackBuilder.cpp
  GlTools.cpp
  GlTriangle.cpp
  GlVertexArrayManager.cpp
  GlVertexArrayVisitor.cpp
  GlXMLTools.cpp
  Glyph.cpp
  GlyphManager.cpp
  OpenGlConfigManager.cpp
  ParametricCurves.cpp
)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ../include/ ${PROJECT_SOURCE_DIR} ${FTGLInclude} ${FREETYPE_INCLUDE_DIRS} ${UTF8CppInclude})
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${JPEG_INCLUDE_DIR} ${PNG_INCLUDE_DIR})

ADD_LIBRARY (${LibTulipOglName} SHARED ${tulip-ogl_LIB_SRCS})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${FTGLLibrary} ${GLEW_LIBRARY} ${LibTulipCoreName} ${OPENGL_glu_LIBRARY} ${OPENGL_gl_LIBRARY} ${FREETYPE_LIBRARY} ${JPEG_LIBRARY} ${PNG_LIBRARY})

INSTALL(TARGETS ${LibTulipOglName}
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_ogl
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_ogl
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_ogl)
