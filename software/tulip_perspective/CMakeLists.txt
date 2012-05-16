SET(SRCS
  src/main.cpp
  src/TulipAgentCommunicator.cpp
  src/TulipPerspectiveMainWindow.cpp
)

QT4_WRAP_CPP(MOC_SRCS
  include/TulipAgentCommunicator.h
  include/TulipPerspectiveMainWindow.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../crash_handling)

IF(WIN32)
INCLUDE_DIRECTORIES(BEFORE ${QT_QTDBUS_INCLUDE_DIR})
ADD_DEFINITIONS(-DQT_DBUS -DQT_NO_DBUS)
ENDIF()

INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR} ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} include/)

ADD_EXECUTABLE(tulip_perspective ${SRCS} ${MOC_SRCS})

IF(LINUX)
  SET_TARGET_PROPERTIES(tulip_perspective PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
ELSEIF(APPLE)
  SET_TARGET_PROPERTIES(tulip_perspective PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
ELSE()
  SET_TARGET_PROPERTIES(tulip_perspective PROPERTIES INSTALL_RPATH "$ORIGIN")
ENDIF()

TARGET_LINK_LIBRARIES(tulip_perspective ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${QT_LIBRARIES} ${QT_QTDBUS_LIBRARY})

TARGET_LINK_LIBRARIES(tulip_perspective crash_handling)


INSTALL(TARGETS tulip_perspective
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_app
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app)
