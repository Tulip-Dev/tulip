if WIN32 
DLL_FLAGS = -no-undefined
MWINDOWS = -mwindows
DLL_TULIP_PLUGINSMANAGER = -DDLL_TULIP_PLUGINSMANAGER
else 
if MACOSX
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif
endif

lib_LTLIBRARIES = libtulip-pluginsmanager.la

bin_PROGRAMS = tulip_check_pl

INCLUDES = ${DLL_TULIP_PLUGINSMANAGER} @TULIP_PLUGINSMANAGER_INCLUDE@ -D_REENTRANT -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED \
	   @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@

libtulip_pluginsmanager_la_LDFLAGS = ${DLL_FLAGS} @GL_LDFLAGS@ @LIB_GL@ @QT_LDFLAGS@ @LIB_QT@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ -release @MAJOR_VERSION@.@MINOR_VERSION@

if LINUX
tulip_check_pl_LDFLAGS = ${MWINDOWS} @TULIP_LDFLAGS@ -ldl
else
tulip_check_pl_LDFLAGS = ${MWINDOWS} @TULIP_LDFLAGS@
endif

AM_CPPFLAGS= @QT_CPPFLAGS@

BUILT_SOURCES = \
	moc_PluginsManagerMainWindow.cpp \
	moc_PluginsManagerDialog.cpp \
	moc_PluginsWidget.cpp \
	moc_PluginsViewWidget.cpp \
	moc_Server.cpp \
	moc_MultiServerManager.cpp \
	moc_HttpRequest.cpp \
	moc_PluginsInfoWidget.cpp \
	moc_UpdatePlugin.cpp \
	moc_InstallPluginDialog.cpp \
	moc_Request.cpp \
	moc_InstallExitDialog.cpp \
	moc_AuthorizationInstallDependencies.cpp \
	moc_ChooseServerDialog.cpp \
	moc_ServersOptionDialog.cpp \
	moc_ModifyServerDialog.cpp \
	moc_PluginsUpdateChecker.cpp \
	moc_ProxyConfigurationDialog.cpp \
	moc_UpdatePluginsDialog.cpp

nodist_libtulip_pluginsmanager_la_SOURCES = $(BUILT_SOURCES)

dist_libtulip_pluginsmanager_la_SOURCES = \
	PluginsManagerMainWindow.cpp \
	PluginsManagerDialog.cpp \
	PluginsWidget.cpp \
	PluginsViewWidget.cpp \
	Server.cpp \
	MultiServerManager.cpp \
	SoapResponseReader.cpp \
	SoapRequestBuilder.cpp \
	HttpRequest.cpp \
	PluginsListManager.cpp \
	Request.cpp \
	PluginsInfoWidget.cpp \
	UpdatePlugin.cpp \
	InstallPluginDialog.cpp \
	InstallExitDialog.cpp \
	AuthorizationInstallDependencies.cpp \
	ChooseServerDialog.cpp \
	ServersOptionDialog.cpp \
	ModifyServerDialog.cpp \
	PluginsUpdateChecker.cpp \
	ProxyConfigurationDialog.cpp \
	UpdatePluginsDialog.cpp \
	PluginsHelp.cpp

dist_tulip_check_pl_SOURCES = TulipCheckPluginLoad.cpp

CLEANFILES = $(BUILT_SOURCES)

moc_%.cpp: $(srcdir)/../include/%.h 
	$(MOC) ${AM_CPPFLAGS} ${INCLUDES} -o $@ $<
