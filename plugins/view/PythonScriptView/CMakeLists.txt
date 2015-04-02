IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

# Force to use SIP headers located in thirdparty and not those installed in the system
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${TulipPythonInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

SET(PythonScriptView_LIB_SRCS
    PythonScriptView.h
    PythonScriptView.cpp
    PythonScriptViewWidget.h
    PythonScriptViewWidget.cpp
)

SET(PythonScriptView_UI
    PythonScriptViewWidget.ui
)

SET(PythonScriptView_MOC
    PythonScriptView.h
    PythonScriptViewWidget.h
)

SET(PythonScriptView_RCC
    PythonScriptViewResource.qrc
)

QTX_WRAP_UI(PythonScriptView_UI_SRCS ${PythonScriptView_UI})
QTX_WRAP_CPP(PythonScriptView_MOC_SRCS ${PythonScriptView_MOC})
QTX_ADD_RESOURCES(PythonScriptView_RCC_SRCS ${PythonScriptView_RCC})

ADD_LIBRARY(PythonScriptView-${TulipVersion} SHARED ${PythonScriptView_LIB_SRCS} ${PythonScriptView_MOC_SRCS} ${PythonScriptView_UI_SRCS} ${PythonScriptView_RCC_SRCS})

TARGET_LINK_LIBRARIES(PythonScriptView-${TulipVersion} ${LibTulipCoreName})
TARGET_LINK_LIBRARIES(PythonScriptView-${TulipVersion} ${LibTulipPythonName})
TARGET_LINK_LIBRARIES(PythonScriptView-${TulipVersion} ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(PythonScriptView-${TulipVersion} ${PYTHON_LIBRARY})


INSTALL_TULIP_PLUGIN(PythonScriptView-${TulipVersion} ${TulipViewPluginsInstallDir})


ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
