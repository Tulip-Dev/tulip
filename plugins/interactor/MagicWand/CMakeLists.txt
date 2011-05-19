QT4_WRAP_UI(UI_HEADERS MagicSelectionInteractorConfigWidget.ui)

QT4_WRAP_CPP(MOC_SRCS MagicSelectionInteractorConfigWidget.h)

SET(SRCS
  InteractorMagicSelection.cpp
  MagicSelectionInteractorConfigWidget.cpp
  MouseMagicWandSelector.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(InteractorMagicSelection-${TulipVersion} SHARED ${SRCS} ${UI_HEADERS} ${MOC_SRCS})
TARGET_LINK_LIBRARIES(InteractorMagicSelection-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

INSTALL(TARGETS InteractorMagicSelection-${TulipVersion}
  DESTINATION ${TulipPluginsInstallDir}/interactors/
  COMPONENT tulip_gui_plugins)
