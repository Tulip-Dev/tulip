BUILT_SOURCES = \
	TulipData.h\
	AppStartUpData.h\
	InfoDialogData.h

nodist_noinst_HEADERS = $(BUILT_SOURCES)

noinst_HEADERS = \
	TulipPluginLoader.h \
	ElementInfoToolTip.h \
	Application.h\
	AppStartUp.h\
	geometrie2D.h\
	InfoDialog.h\
	TulipApp.h

CLEANFILES = $(BUILT_SOURCES)

%Data.h : ../designer/%.ui
	$(UIC) -o $@ $<
