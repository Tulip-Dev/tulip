
# IF(${COMPILE_EXAMPLES})

#  INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude})
  
#  FIND_PACKAGE(GLUT)
#  IF(GLUT_FOUND)
#    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
#    ADD_EXECUTABLE(glutViewer glutviewer/glutviewer.cpp)
#    TARGET_LINK_LIBRARIES(glutViewer ${OPENGL_gl_LIBRARY} glut ${LibTulipCoreName} ${LibTulipOglName})
#  ELSE()
#    MESSAGE("GLUT not found, glutViewer will not be built")
#  ENDIF()

#  QT4_WRAP_CPP(MOC_SRCS qtviewer/mainwindow.h)
#  ADD_EXECUTABLE(qtViewer qtviewer/qtviewer.cpp qtviewer/mainwindow.cpp ${MOC_SRCS})
#  TARGET_LINK_LIBRARIES(qtViewer ${LibTulipCoreName} ${LibTulipGUIName})
  
# ENDIF()
