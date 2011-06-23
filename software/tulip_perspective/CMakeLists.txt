SET(SRCS
  src/main.cpp
  src/TulipAgentCommunicator.cpp
)

QT4_WRAP_CPP(MOC_SRCS
  include/TulipAgentCommunicator.h
)

IF(USE_GOOGLE_BREAKPAD)
  INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/google-breakpad/src)
  ADD_DEFINITIONS(-DUSE_GOOGLE_BREAKPAD_CLIENT)
ENDIF(USE_GOOGLE_BREAKPAD)
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR} ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} include/)

ADD_EXECUTABLE(tulip_perspective ${SRCS} ${MOC_SRCS})

IF(LINUX)
  SET_TARGET_PROPERTIES(tulip_perspective PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
ELSEIF(APPLE)
  SET_TARGET_PROPERTIES(tulip_perspective PROPERTIES INSTALL_RPATH "$ORIGIN/../Frameworks")
ELSE()
  SET_TARGET_PROPERTIES(tulip_perspective PROPERTIES INSTALL_RPATH "$ORIGIN")
ENDIF()

TARGET_LINK_LIBRARIES(tulip_perspective ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${QT_QTDBUS_LIBRARY})

IF(TULIP_USE_TULIP3)
  TARGET_LINK_LIBRARIES(tulip_perspective ${LibTulip3CompatName})
ENDIF()

IF(USE_GOOGLE_BREAKPAD)
  TARGET_LINK_LIBRARIES(tulip_perspective breakpad_client)
ENDIF(USE_GOOGLE_BREAKPAD)


INSTALL(TARGETS tulip_perspective
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_app
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app)
