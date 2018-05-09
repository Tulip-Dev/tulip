IF(NOT TULIP_BUILD_FOR_APPIMAGE)

INSTALL(FILES
  tulip/PythonInterpreter.h
  tulip/PythonInterpreter.cxx
  tulip/PythonCodeEditor.h
  tulip/PythonShellWidget.h
  tulip/APIDataBase.h
  tulip/AutoCompletionDataBase.h
  tulip/PythonEditorsTabWidget.h
  tulip/PythonCppTypesConverter.h
  tulip/PythonIDE.h
DESTINATION ${TulipIncludeInstallDir}/tulip/
COMPONENT tulip_python
)

ENDIF(NOT TULIP_BUILD_FOR_APPIMAGE)
