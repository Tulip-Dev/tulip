QT4_WRAP_UI(UI_HEADERS MagicSelectionInteractorConfigWidget.ui)

QT4_WRAP_CPP(MOC_SRCS MagicSelectionInteractorConfigWidget.h)

SET(SRCS
  InteractorMagicSelection.cpp
  MagicSelectionInteractorConfigWidget.cpp
  MouseMagicWandSelector.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(InteractorMagicSelection-${TulipVersion} SHARED ${SRCS} ${UI_HEADERS} ${MOC_SRCS})
TARGET_LINK_LIBRARIES(InteractorMagicSelection-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

INSTALL(TARGETS InteractorMagicSelection-${TulipVersion}
        DESTINATION ${TulipPluginsInstallDir}/interactors/
        COMPONENT libtulip_qt_plugins)
