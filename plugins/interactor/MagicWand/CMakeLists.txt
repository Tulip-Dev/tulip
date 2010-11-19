TULIP_QT4_WRAP_UI(InteractorMagicSelection_UI_SRCS MagicSelectionInteractorConfigWidget.ui)

TULIP_QT4_WRAP_CPP(InteractorMagicSelection_MOC_SRCS MagicSelectionInteractorConfigWidget.h)

SET(InteractorMagicSelection_SRCS
    InteractorMagicSelection.cpp
    MagicSelectionInteractorConfigWidget.cpp
    MouseMagicWandSelector.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(InteractorMagicSelection-${TulipVersion} SHARED ${InteractorMagicSelection_SRCS} ${InteractorMagicSelection_UI_SRCS} ${InteractorMagicSelection_MOC_SRCS})
TARGET_LINK_LIBRARIES(InteractorMagicSelection-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})
ADD_DEPENDENCIES(InteractorMagicSelection-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

INSTALL(TARGETS InteractorMagicSelection-${TulipVersion} DESTINATION ${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}tlp/interactors/)