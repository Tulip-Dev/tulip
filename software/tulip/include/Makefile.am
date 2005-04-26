UIC = LD_LIBRARY_PATH=$(top_builddir)/library/tulip/src/.libs:$(top_builddir)/library/tulip-ogl/src/.libs:$(top_builddir)/library/tulip-qt/src/.libs:$$LD_LIBRARY_PATH @UIC@ -L @TULIP_QTDESIGNERPLUGIN@

BUILT_SOURCES = \
	QtProgressData.h \
	TulipData.h\
	OverviewData.h\
	TabWidgetData.h\
	AppStartUpData.h\
	InfoDialogData.h\
	NavigateGlGraphData.h\
	PropertyDialogData.h\
	ToolBarData.h\
	View3DSetupData.h

nodist_noinst_HEADERS = \
	QtProgressData.h \
	TulipData.h\
	OverviewData.h\
	TabWidgetData.h\
	AppStartUpData.h\
	InfoDialogData.h\
	NavigateGlGraphData.h\
	PropertyDialogData.h\
	ToolBarData.h\
	View3DSetupData.h

noinst_HEADERS = \
	Overview.h\
	MouseTreeFishEyes.h \
	ElementInfoToolTip.h \
	AppDialog.h\
	Application.h\
	AppStartUp.h\
	geometrie2D.h\
	InfoDialog.h\
	MouseAddEdge.h\
	MouseAddNode.h\
	MouseMoveSelection.h\
	MouseSelection.h\
	MouseMagicSelection.h\
	NavigateButton.h\
	NavigateGlGraph.h\
	PropertyDialog.h\
	ToolBar.h\
	View3DSetup.h\
	QtProgress.h\
	viewGl.h

CLEANFILES = $(BUILT_SOURCES)

%Data.h : ../designer/%.ui
	$(UIC) -o $@ $<
