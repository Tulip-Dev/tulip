SET(SRCS
  src/main.cpp
  src/TulipPerspectiveMainWindow.cpp
)

QTX_WRAP_CPP(MOC_SRCS
  include/TulipPerspectiveMainWindow.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../crash_handling)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} include/)

IF(USE_QT5)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
ENDIF(USE_QT5)

ADD_EXECUTABLE(tulip_perspective ${SRCS} ${MOC_SRCS})

IF(LINUX)
  SET_TARGET_PROPERTIES(tulip_perspective PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
ELSEIF(APPLE)
  SET_TARGET_PROPERTIES(tulip_perspective PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
ELSE()
  SET_TARGET_PROPERTIES(tulip_perspective PROPERTIES INSTALL_RPATH "$ORIGIN")
ENDIF()

TARGET_LINK_LIBRARIES(tulip_perspective crash_handling)

TARGET_LINK_LIBRARIES(tulip_perspective ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${QT_LIBRARIES})

INSTALL(TARGETS tulip_perspective
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_app
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app)
