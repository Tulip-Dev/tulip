INSTALL(DIRECTORY bitmaps DESTINATION share/tulip/
PATTERN ".svn" EXCLUDE)

SET(SRCS
  src/AboutTulipPage.cpp
  src/main.cpp
  src/PluginsCenter.cpp
  src/RssParser.cpp
  src/TulipMainWindow.cpp
  src/TulipSplashScreen.cpp
  src/TulipWelcomePage.cpp
)

QT4_WRAP_UI(UI_HEADERS
  designer/AboutTulipPage.ui
  designer/PluginsCenter.ui
  designer/TulipMainWindow.ui
  designer/TulipWelcomePage.ui
)

QT4_WRAP_CPP(MOC_SRCS
  include/PluginsCenter.h
  include/TulipMainWindow.h
  include/TulipWelcomePage.h
)

QT4_ADD_RESOURCES(RCC_SRCS
  designer/TulipApp.qrc
)

INCLUDE_DIRECTORIES(include ${CMAKE_CURRENT_BINARY_DIR})

ADD_EXECUTABLE(tulip_app ${SRCS} ${RCC_SRCS} ${UI_HEADERS} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(tulip_app ${TULIP_LIBS} ${QT_LIBRARIES})

INSTALL(TARGETS tulip_app
      DESTINATION bin)
