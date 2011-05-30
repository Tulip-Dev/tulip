INCLUDE_DIRECTORIES(${QT_INCLUDE_DIRECTORY} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

ADD_DEFINITIONS(-DQT_BUILD_DBUS_LIB -DDBUS_API_SUBJECT_TO_CHANGE -DQT_MAKEDLL)

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
qdbusmarshaller.cpp
qdbuscontext.cpp
qdbuspendingcall.cpp
qdbuspendingreply.cpp
qdbus_symbols.cpp
qdbusservicewatcher.cpp)

QT4_GENERATE_MOC(qdbusabstractinterface.h moc_qdbusabstractinterface.cpp)
QT4_GENERATE_MOC(qdbuspendingcall.h moc_qdbuspendingcall.cpp)
QT4_GENERATE_MOC(qdbusservicewatcher.h moc_qdbusservicewatcher.cpp)

QT4_WRAP_CPP(MOC_SRCS
qdbusabstractadaptor.h
qdbusconnection_p.h
qdbusconnectioninterface.h
qdbuspendingcall_p.h
qdbusserver.h)

ADD_CUSTOM_TARGET(force_qdbus_moc_generation ALL SOURCES moc_qdbusabstractinterface.cpp moc_qdbuspendingcall.cpp moc_qdbusservicewatcher.cpp)

ADD_LIBRARY(QtDBus4 SHARED ${SRCS} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(QtDBus4 ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} -ldbus-1 -lws2_32 -ladvapi32 -lnetapi32 -luser32)

INSTALL(TARGETS QtDBus4
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT dbus
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT dbus
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT dbus)

# Override the Qt's library variable with the compiled target name
SET(QT_QTDBUS_LIBRARY QtDBus4)
