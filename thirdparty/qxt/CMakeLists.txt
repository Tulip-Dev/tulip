INCLUDE_DIRECTORIES(src)

ADD_DEFINITIONS("-DBUILD_QXT_GUI -UQXT_STATIC")
IF(NOT MSVC)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF(NOT MSVC)

SET(SRCS
  src/qxtspanslider.cpp
  src/qxtflowview.cpp
  src/qxtflowview_p.cpp
  src/qxtglobal.cpp
)

QTX_WRAP_CPP(MOC_SRCS
src/qxtspanslider.h
src/qxtspanslider_p.h
src/qxtflowview_p.h
src/qxtflowview.h
)

ADD_LIBRARY(${QxtLibrary} SHARED ${SRCS} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(${QxtLibrary} ${QT_LIBRARIES})
SET_TARGET_PROPERTIES(${QxtLibrary} PROPERTIES
                      SOVERSION 0)

INSTALL(TARGETS ${QxtLibrary} RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT qxt
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT qxt
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT qxt)
