FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

MACRO (TULIP_QT4_WRAP_UI outfiles )
  QT4_EXTRACT_OPTIONS(ui_files ui_options ${ARGN})
  FOREACH (it ${ui_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/../include/${outfile}Data.h)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_UIC_EXECUTABLE}
      ARGS ${ui_options} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH (it)
ENDMACRO (TULIP_QT4_WRAP_UI)

MACRO (TULIP_QT4_WRAP_CPP outfiles )
  QT4_GET_MOC_FLAGS(moc_flags)
  QT4_EXTRACT_OPTIONS(moc_files moc_options ${ARGN})

  FOREACH (it ${moc_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(it ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/moc_${outfile}.cpp)
    QT4_CREATE_MOC_COMMAND(${it} ${outfile} "${moc_flags}" "${moc_options}")
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH(it)

ENDMACRO (TULIP_QT4_WRAP_CPP)	

set(tulip-pluginsmanager_UI
     ../designer/ProxyConfigurationDialog.ui)

set(tulip-pluginsmanager_MOC
    ../include/PluginsManagerMainWindow.h
    ../include/PluginsManagerDialog.h
    ../include/PluginsWidget.h
    ../include/PluginsViewWidget.h
    ../include/Server.h
    ../include/MultiServerManager.h
    ../include/HttpRequest.h
    ../include/PluginsInfoWidget.h
    ../include/UpdatePlugin.h
    ../include/InstallPluginDialog.h
    ../include/Request.h
    ../include/InstallExitDialog.h
    ../include/AuthorizationInstallDependencies.h
    ../include/ChooseServerDialog.h
    ../include/ServersOptionDialog.h
    ../include/ModifyServerDialog.h
    ../include/PluginsUpdateChecker.h
    ../include/ProxyConfigurationDialog.h )

set(tulip-pluginsmanager_LIB_SRCS 
    PluginsManagerMainWindow.cpp
    PluginsManagerDialog.cpp
    PluginsWidget.cpp
    PluginsViewWidget.cpp
    Server.cpp
    MultiServerManager.cpp
    SoapResponseReader.cpp
    SoapRequestBuilder.cpp
    HttpRequest.cpp
    PluginsListManager.cpp
    Request.cpp 
    PluginsInfoWidget.cpp
    UpdatePlugin.cpp
    InstallPluginDialog.cpp
    InstallExitDialog.cpp
    AuthorizationInstallDependencies.cpp
    ChooseServerDialog.cpp
    ServersOptionDialog.cpp
    ModifyServerDialog.cpp
    PluginsUpdateChecker.cpp
    ProxyConfigurationDialog.cpp )

TULIP_QT4_WRAP_UI(tulip-pluginsmanager_UI_SRCS ${tulip-pluginsmanager_UI})
TULIP_QT4_WRAP_CPP(tulip-pluginsmanager_MOC_SRCS ${tulip-pluginsmanager_MOC})

INCLUDE_DIRECTORIES(../include/ ${TULIP_INCLUDE} ${TULIP_QT_INCLUDE})

ADD_LIBRARY(tulip-pluginsmanager SHARED ${tulip-pluginsmanager_UI_SRCS} ${tulip-pluginsmanager_MOC_SRCS} ${tulip-pluginsmanager_LIB_SRCS})

########### install files ###############

INSTALL_TARGETS(${TULIP_LIB_INSTALL_DIR} tulip-pluginsmanager)


#original Makefile.am contents follow:

#if WIN32 
#DLL_FLAGS = -no-undefined
#MWINDOWS = -mwindows
#DLL_TULIP_PLUGINSMANAGER = -DDLL_TULIP_PLUGINSMANAGER
#else 
#if MACOSX
#DLL_FLAGS = -Wl,-headerpad_max_install_names
#endif
#endif
#
#lib_LTLIBRARIES = libtulip-pluginsmanager.la
#
#bin_PROGRAMS = tulip_check_pl
#
#INCLUDES = ${DLL_TULIP_PLUGINSMANAGER} @TULIP_PLUGINSMANAGER_INCLUDE@ -D_REENTRANT -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED \
#	   @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@
#
#libtulip_pluginsmanager_la_LDFLAGS = ${DLL_FLAGS} @GL_LDFLAGS@ @LIB_GL@ @QT_LDFLAGS@ @LIB_QT@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ -release @MAJOR_VERSION@.@MINOR_VERSION@
#
#if LINUX
#tulip_check_pl_LDFLAGS = ${MWINDOWS} @TULIP_LDFLAGS@ -ldl
#else
#tulip_check_pl_LDFLAGS = ${MWINDOWS} @TULIP_LDFLAGS@
#endif
#
#AM_CPPFLAGS= @QT_CPPFLAGS@
#
#BUILT_SOURCES = \
#	moc_PluginsManagerMainWindow.cpp \
#	moc_PluginsManagerDialog.cpp \
#	moc_PluginsWidget.cpp \
#	moc_PluginsViewWidget.cpp \
#	moc_Server.cpp \
#	moc_MultiServerManager.cpp \
#	moc_HttpRequest.cpp \
#	moc_PluginsInfoWidget.cpp \
#	moc_UpdatePlugin.cpp \
#	moc_InstallPluginDialog.cpp \
#	moc_Request.cpp \
#	moc_InstallExitDialog.cpp \
#	moc_AuthorizationInstallDependencies.cpp \
#	moc_ChooseServerDialog.cpp \
#	moc_ServersOptionDialog.cpp \
#	moc_ModifyServerDialog.cpp \
#	moc_PluginsUpdateChecker.cpp \
#	moc_ProxyConfigurationDialog.cpp
#
#nodist_libtulip_pluginsmanager_la_SOURCES = $(BUILT_SOURCES)
#
#dist_libtulip_pluginsmanager_la_SOURCES = \
#	PluginsManagerMainWindow.cpp \
#	PluginsManagerDialog.cpp \
#	PluginsWidget.cpp \
#	PluginsViewWidget.cpp \
#	Server.cpp \
#	MultiServerManager.cpp \
#	SoapResponseReader.cpp \
#	SoapRequestBuilder.cpp \
#	HttpRequest.cpp \
#	PluginsListManager.cpp \
#	Request.cpp \
#	PluginsInfoWidget.cpp \
#	UpdatePlugin.cpp \
#	InstallPluginDialog.cpp \
#	InstallExitDialog.cpp \
#	AuthorizationInstallDependencies.cpp \
#	ChooseServerDialog.cpp \
#	ServersOptionDialog.cpp \
#	ModifyServerDialog.cpp \
#	PluginsUpdateChecker.cpp \
#	ProxyConfigurationDialog.cpp
#
#dist_tulip_check_pl_SOURCES = TulipCheckPluginLoad.cpp
#
#CLEANFILES = $(BUILT_SOURCES)
#
#moc_%.cpp: $(srcdir)/../include/%.h 
#	$(MOC) ${AM_CPPFLAGS} ${INCLUDES} -o $@ $<
