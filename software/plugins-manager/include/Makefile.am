BUILT_SOURCES = \
	PluginLoaderWithInfo.h \
	PluginInfo.h \
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
	CheckUpdate.h \
	PluginsUpdateChecker.h \
	ProxyConfigurationDialog.h \
	ProxyConfigurationDialogData.h

noinst_HEADERS = $(BUILT_SOURCES)

%Data.h: ../designer/%.ui
	$(UIC) -o $@ $<
