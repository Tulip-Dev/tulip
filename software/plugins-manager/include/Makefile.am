UIC = LD_LIBRARY_PATH=$(top_builddir)/library/tulip/src/.libs:$(top_builddir)/library/tulip-ogl/src/.libs:$(top_builddir)/library/tulip-qt/src/.libs:$$LD_LIBRARY_PATH @UIC@ -L @TULIP_QTDESIGNERPLUGIN@

BUILT_SOURCES = \
	PluginLoaderWithInfo.h \
	PluginsManagerMainWindow.h \
	PluginsManagerDialog.h \
	PluginsWidget.h \
	PluginsViewWidget.h \
	Server.h \
	MultiServerManager.h \
	SoapResponseReader.h \
	SoapRequestBuilder.h \
	Request.h \
	ResponseTreatment.h \
	HttpRequest.h \
	PluginsListManager.h \
	TPWFonctor.h \
	PluginsInfoWidget.h \
	UpdatePlugin.h \
	InstallPluginDialog.h \
	InstallExitDialog.h \
	AuthorizationInstallDependencies.h \
	ChooseServerDialog.h \
	ServersOptionDialog.h \
	ModifyServerDialog.h \
	PluginsUpdateChecker.h

nodist_noinst_HEADERS = $(BUILT_SOURCES)

noinst_HEADERS =

CLEANFILES = $(BUILT_SOURCES)
