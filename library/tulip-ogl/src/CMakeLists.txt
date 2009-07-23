set(tulip-ogl_LIB_SRCS 
	GlGraphRenderingParameters.cpp
	GlTools.cpp 
	Curves.cpp 
	Hline.cpp 
	Parser.cpp 
	Paragraph.cpp 
	Renderer.cpp 
	GlRenderer.cpp 
	TextRenderer.cpp 
	Table.cpp 
	Context.cpp 
	Document.cpp 
	Glyph.cpp 
	CubeOutLined.cpp 
	Camera.cpp 
	GlGraphStaticData.cpp 
	GlGrid.cpp 
	GlLabel.cpp 
	GlComposite.cpp 
	GlGraphComposite.cpp 
	GlCircle.cpp 
	GlRect.cpp 
	GlRectTextured.cpp 
	GlPolygon.cpp 
	GlComplexPolygon.cpp 
	GlMultiPolygon.cpp 
	GlSphere.cpp 
	GlLayer.cpp 
	GlCPULODCalculator.cpp 
	GlLODSceneVisitor.cpp 
	GlBoundingBoxSceneVisitor.cpp 
	GlSelectSceneVisitor.cpp 
	GlBox.cpp 
	GlScene.cpp 
	GlTextureManager.cpp 
	GlConvexHull.cpp 
	GlHierarchyConvexHulls.cpp 
	Bezier.cpp 
	Spline.cpp 
   	GlLines.cpp 
	TLPPixmapFont.cpp 
	TLPPixmapGlyph.cpp 
   	EpsFunction.cpp 
	GlSVGFeedBackBuilder.cpp 
	GlEPSFeedBackBuilder.cpp 
	GlTLPFeedBackBuilder.cpp 
	GlFeedBackRecorder.cpp 
	GlyphManager.cpp 
	GlGraphInputData.cpp 
	GlNode.cpp 
	GlMetaNode.cpp 
	GlEdge.cpp 
	GlMetaNodeTrueRenderer.cpp 
	GlDisplayListManager.cpp 
	GlewManager.cpp 
	GlSceneObserver.cpp 
	GlLine.cpp 
	GlCurve.cpp 
	GlQuad.cpp 
	GlPolyQuad.cpp 
	GlAxis.cpp 
	GlQuantitativeAxis.cpp 
	GlNominativeAxis.cpp 
	GlProgressBar.cpp 
	GlXMLTools.cpp 
	GlColorScale.cpp 
	GlPointManager.cpp 
	FastBezier.cpp 
	GlBezierCurve.cpp 
	GlCatmullRomCurve.cpp 
	GlShaderManager.cpp
)

INCLUDE_DIRECTORIES( ${TulipInclude} ../include/ ${PROJECT_SOURCE_DIR} ${GZStreamInclude} ${GLEInclude} ${FTGLInclude} ${LIBXML2_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR_freetype2})
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

ADD_LIBRARY (${LibTulipOglName} SHARED ${tulip-ogl_LIB_SRCS})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ftgl)
TARGET_LINK_LIBRARIES(${LibTulipOglName} gle)
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${GLEW_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${LibTulipName})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${OPENGL_glu_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${OPENGL_gl_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${LIBXML2_LIBRARIES})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${JPEG_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${PNG_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipOglName} ${FREETYPE_LIBRARY})
ADD_DEPENDENCIES(${LibTulipOglName} ftgl gle ${LibTulipName})

INSTALL_TARGETS(${TulipLibInstallDir} ${LibTulipOglName})

