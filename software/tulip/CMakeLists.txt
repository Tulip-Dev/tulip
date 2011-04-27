INSTALL(DIRECTORY bitmaps DESTINATION share/tulip/
PATTERN ".svn" EXCLUDE)

SET(SRCS
  src/main.cpp
  src/TulipSplashScreen.cpp
)

INCLUDE_DIRECTORIES(include)

ADD_EXECUTABLE(tulip_app ${SRCS})

TARGET_LINK_LIBRARIES(tulip_app ${TULIP_LIBS} ${QT_LIBRARIES})

INSTALL(TARGETS tulip_app
      DESTINATION bin)
