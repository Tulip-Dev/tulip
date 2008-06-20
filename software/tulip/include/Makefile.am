BUILT_SOURCES = \
	QtProgressData.h \
	TulipData.h\
	AppStartUpData.h\
	InfoDialogData.h\
	GridOptionsData.h

nodist_noinst_HEADERS = $(BUILT_SOURCES)

noinst_HEADERS = \
	TulipPluginLoader.h \
	ElementInfoToolTip.h \
	Application.h\
	AppStartUp.h\
	geometrie2D.h\
	InfoDialog.h\
	QtProgress.h\
	viewGl.h \
	GridOptionsWidget.h

CLEANFILES = $(BUILT_SOURCES)

%Data.h : ../designer/%.ui
	$(UIC) -o $@ $<
