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
 GlGraphComposite.cpp
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
 GlMetaNode.cpp
 GlMetaNodeTrueRenderer.cpp
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
 GlRectTextured.cpp
 GlRegularPolygon.cpp
 GlScene.cpp
 GlSceneObserver.cpp
 GlSceneZoomAndPan.cpp
 GlSelectSceneVisitor.cpp
 GlShaderProgram.cpp
 GlSimpleEntity.cpp
 GlSphere.cpp
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

INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ../include/ ${PROJECT_SOURCE_DIR} ${FTGLInclude} ${LIBXML2_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR}
${JPEG_INCLUDE_DIR} ${PNG_INCLUDE_DIR})

IF(ENABLE_RENDERING_TIME_DISPLAY)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_RENDERING_TIME_DISPLAY")
ENDIF(ENABLE_RENDERING_TIME_DISPLAY)

ADD_LIBRARY (${LibTulipOglName} SHARED ${tulip-ogl_LIB_SRCS})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ftgl)
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${GLEW_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${LibTulipName})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${OPENGL_glu_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${OPENGL_gl_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${LIBXML2_LIBRARIES})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${JPEG_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${PNG_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${FREETYPE_LIBRARY})
ADD_DEPENDENCIES(${LibTulipOglName} ftgl ${LibTulipName})

INSTALL(TARGETS ${LibTulipOglName}
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT libtulip_ogl
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT libtulip_ogl
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT libtulip_ogl)

IF(WIN32)
  FIND_LIBRARY_GLOB(XML_INSTALL_PATH "libxml*.dll")
  FIND_LIBRARY_GLOB(FREETYPE_INSTALL_PATH "freetype*.dll")
  FIND_LIBRARY_GLOB(GLEW_INSTALL_PATH "glew*.dll")
  FIND_LIBRARY_GLOB(JPEG_INSTALL_PATH "jpeg*.dll")
  FIND_LIBRARY_GLOB(PNG_INSTALL_PATH "libpng*.dll")

  INSTALL_EXTERNAL_LIB(XML_INSTALL_PATH libtulip_ogl)
  INSTALL_EXTERNAL_LIB(FREETYPE_INSTALL_PATH libtulip_ogl)
  INSTALL_EXTERNAL_LIB(GLEW_INSTALL_PATH libtulip_ogl)
  INSTALL_EXTERNAL_LIB(JPEG_INSTALL_PATH libtulip_ogl)
  INSTALL_EXTERNAL_LIB(PNG_INSTALL_PATH libtulip_ogl)
ELSEIF(APPLE)
#Search and install external libraries
#no need of libxml2 which already exists on standard installation
#INSTALL_EXTERNAL_LIB(LIBXML2_LIBRARIES libtulip)
INSTALL_EXTERNAL_LIB(FREETYPE_LIBRARY libtulip_ogl)
INSTALL_EXTERNAL_LIB(GLEW_LIBRARY libtulip_ogl)
INSTALL_EXTERNAL_LIB(JPEG_LIBRARY libtulip_ogl)

#Hack PNG_LIBRARY is not the library but the framework.
IF(IS_DIRECTORY ${PNG_LIBRARY})
FILE(GLOB PNG_PATH "${PNG_LIBRARY}/libpng*")
ELSE()
SET(PNG_PATH ${PNG_LIBRARY})
ENDIF()
INSTALL_EXTERNAL_LIB(PNG_PATH libtulip_ogl)
ENDIF()
