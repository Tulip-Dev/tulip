SET(SRCS
)

QT4_WRAP_UI(UI_HEADERS
)

QT4_WRAP_CPP(MOC_SRCS
)

QT4_ADD_RESOURCES(RCC_SRCS )

INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR} ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${TulipGUIBuildInclude} ${Tulip3CompatInclude} ${Tulip3CompatBuildInclude} ${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(${LibTulip3CompatName} SHARED ${SRCS} ${MOC_SRCS} ${RCC_SRCS} ${UI_HEADERS})

TARGET_LINK_LIBRARIES(${LibTulip3CompatName} ${QT_LIBRARIES} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

INSTALL(TARGETS ${LibTulip3CompatName}
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip3_compat
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip3_compat
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip3_compat)
