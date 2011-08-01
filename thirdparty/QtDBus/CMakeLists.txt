# Sources from this directory are direcly taken from the src/dbus directory of Qt framework.
# This CMakeLists is not shipped along with those sources since Qt is compiled using qmake.
# To get the sources compiling, make sure that every file including a moc uses the .cxx file extension (rename #include "moc_xxx.cpp" to #include "moc_xxx.cxx")
# Included mocs should not be compiled and are put into a separate QT4_WRAP_CPP macro.
# no other modification should be made on the sources.

INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/../)

INCLUDE_DIRECTORIES(${QT_INCLUDE_DIRECTORY} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${DBUS_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../dbus/libdbus/cmake/)

ADD_DEFINITIONS(-DQT_BUILD_DBUS_LIB -DDBUS_API_SUBJECT_TO_CHANGE -DQT_MAKEDLL -DQT_DBUS -DQT_NO_DBUS)

SET(SRCS
qdbusconnection.cpp
qdbusconnectioninterface.cpp
qdbuserror.cpp
qdbusintegrator.cpp
qdbusmessage.cpp
qdbusserver.cpp
qdbusabstractinterface.cpp
qdbusinterface.cpp
qdbusxmlparser.cpp
qdbusutil.cpp
qdbusintrospection.cpp
qdbusabstractadaptor.cpp
qdbusinternalfilters.cpp
qdbusmetaobject.cpp
qdbusxmlgenerator.cpp
qdbusmisc.cpp
qdbusargument.cpp
qdbusreply.cpp
qdbusmetatype.cpp
qdbusextratypes.cpp
qdbuscontext.cpp
qdbuspendingcall.cpp
qdbuspendingreply.cpp
qdbus_symbols.cpp
qdbusservicewatcher.cpp)

#Those mocs are not compiled since they are included in source files.
QT4_WRAP_CPP(MOC_SRCS_TMP
qdbusservicewatcher.h
qdbuspendingcall.h
qdbusabstractinterface.h)

QT4_WRAP_CPP(MOC_SRCS
qdbusabstractadaptor.h
qdbusconnection_p.h
qdbusconnectioninterface.h
qdbuspendingcall_p.h
qdbusserver.h)

DISABLE_COMPILER_WARNINGS()

ADD_LIBRARY(QtDBus4 SHARED ${SRCS} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(QtDBus4 ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} ${DBUS_LIBRARY})
IF(WIN32)
  TARGET_LINK_LIBRARIES(QtDBus4 ws2_32 advapi32 netapi32 user32)
ENDIF(WIN32)

#if this is not specified, the compilation might try to go ahead without generating the mocs
ADD_CUSTOM_TARGET(force_qdbus_moc_generation ALL SOURCES ${MOC_SRCS_TMP})
ADD_DEPENDENCIES(QtDBus4 force_qdbus_moc_generation)

INSTALL(TARGETS QtDBus4
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT dbus
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT dbus
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT dbus)

# Override the Qt's library variable with the compiled target name
SET(QT_QTDBUS_LIBRARY QtDBus4 CACHE STRING "" FORCE)
