IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

# Force to use SIP headers located in thirdparty and not those installed in the system
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${TulipPythonInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

SET(pythonscriptview_LIB_SRCS
    PythonScriptView.h
    PythonScriptView.cpp
    PythonScriptViewWidget.h
    PythonScriptViewWidget.cpp
)

SET(pythonscriptview_UI
    PythonScriptViewWidget.ui
)

SET(pythonscriptview_MOC
    PythonScriptView.h
    PythonScriptViewWidget.h
)

SET(pythonscriptview_RCC
    PythonScriptViewResource.qrc
)

QTX_WRAP_UI(pythonscriptview_UI_SRCS ${pythonscriptview_UI})
QTX_WRAP_CPP(pythonscriptview_MOC_SRCS ${pythonscriptview_MOC})
QTX_ADD_RESOURCES(pythonscriptview_RCC_SRCS ${pythonscriptview_RCC})

ADD_LIBRARY(pythonscriptview-${TulipVersion} SHARED ${pythonscriptview_LIB_SRCS} ${pythonscriptview_MOC_SRCS} ${pythonscriptview_UI_SRCS} ${pythonscriptview_RCC_SRCS})

TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipCoreName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipPythonName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${PYTHON_LIBRARY})


INSTALL(TARGETS pythonscriptview-${TulipVersion} DESTINATION ${TulipViewPluginsInstallDir} COMPONENT tulip_python)


ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
