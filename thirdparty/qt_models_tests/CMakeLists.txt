SET(SRCS
dynamictreemodel.cpp
modeltest.cpp)
#tst_modeltest.cpp)

QT4_WRAP_CPP(MOCS
dynamictreemodel.h
modeltest.h)

ADD_LIBRARY(qt4_test_models SHARED ${SRCS} ${MOCS})

TARGET_LINK_LIBRARIES(qt4_test_models ${QT_LIBRARIES})

IF(MSVC)
  ADD_DEFINITIONS("-DMODEL_TESTS_DLL")
ENDIF()

INSTALL(TARGETS qt4_test_models
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT model_tests
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT model_tests
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT model_tests)
