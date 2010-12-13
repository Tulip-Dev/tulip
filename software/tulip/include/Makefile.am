EXTRA_DIST = CMakeLists.txt

BUILT_SOURCES = \
	TulipData.h \
	AppStartUpData.h \
	InfoDialogData.h \
	PreferenceDialogData.h \
	ChooseControllerDialogData.h

nodist_noinst_HEADERS = $(BUILT_SOURCES)

noinst_HEADERS = \
	TulipPluginLoader.h \
	ElementInfoToolTip.h \
	Application.h\
	AppStartUp.h\
	InfoDialog.h\
	PreferenceDialog.h\
	ChooseControllerDialog.h\
	GraphNeedsSavingObserver.h\
	TulipApp.h

CLEANFILES = $(BUILT_SOURCES)

%Data.h : ../designer/%.ui
	$(UIC) -o $@ $<
