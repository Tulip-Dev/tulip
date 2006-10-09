UIC = LD_LIBRARY_PATH=$(top_builddir)/library/tulip/src/.libs:$(top_builddir)/library/tulip-ogl/src/.libs:$(top_builddir)/library/tulip-qt/src/.libs:$$LD_LIBRARY_PATH @UIC@ -L @TULIP_QTDESIGNERPLUGIN@

BUILT_SOURCES = \
	QtProgressData.h \
	TulipData.h\
	TabWidgetData.h\
	AppStartUpData.h\
	InfoDialogData.h\
	PropertyDialogData.h\
	ToolBarData.h\
	TulipStatsData.h\
	GridOptionsData.h

nodist_noinst_HEADERS = \
	QtProgressData.h \
	TulipData.h\
	TabWidgetData.h\
	AppStartUpData.h\
	InfoDialogData.h\
	PropertyDialogData.h\
	ToolBarData.h\
	TulipStatsData.h\
	GridOptionsData.h

noinst_HEADERS = \
	ElementInfoToolTip.h \
	Application.h\
	AppStartUp.h\
	geometrie2D.h\
	InfoDialog.h\
	PropertyDialog.h\
	PropertyDialogQt3.h \
	ToolBar.h\
	QtProgress.h\
	viewGl.h \
	TulipStatsWidget.h\
	GridOptionsWidget.h

CLEANFILES = $(BUILT_SOURCES)

%Data.h : ../designer/%.ui
	$(UIC) -o $@ $<
