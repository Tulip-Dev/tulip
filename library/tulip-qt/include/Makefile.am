nobase_include_HEADERS = \
	tulip/TulipTableWidget.h \
	tulip/GlMainWidget.h \
	tulip/TabWidget.h \
	tulip/PropertyDialog.h \	
	tulip/CopyPropertyDialog.h \
	tulip/TulipStatsWidget.h \
	tulip/RenderingParametersDialog.h \
	tulip/GlMainView.h \	
	tulip/GWInteractor.h \
	tulip/MouseSelector.h \
	tulip/MouseBoxZoomer.h\
	tulip/MouseEdgeBuilder.h\
	tulip/MouseInteractors.h \
	tulip/MouseMagicSelector.h\
	tulip/MouseNodeBuilder.h\
	tulip/MouseSelectionEditor.h\
	tulip/MouseEdgeSelector.h \
	tulip/MouseEdgeBendEditor.h \
	tulip/MouseShowElementInfos.h \
	tulip/PropertyWidget.h \
	tulip/ElementPropertiesWidgetUI.h \
	tulip/ElementPropertiesWidget.h \
	tulip/FindSelectionWidget.h \
	tulip/SGHierarchyWidget.h \
	tulip/GWOverviewWidget.h \
	tulip/LayerManagerWidget.h \
	tulip/LayerManagerWidgetData.h \
	tulip/TlpQtTools.h \
	tulip/SGHierarchyWidgetPlugin.h \
	tulip/GlMainWidgetPlugin.h \
	tulip/ElementPropertiesWidgetPlugin.h \
	tulip/GWOverviewWidgetPlugin.h \
	tulip/PropertyWidgetPlugin.h \
	tulip/QtCPULODCalculator.h \
	tulip/InteractorManager.h \
	tulip/View.h \
	tulip/ViewPluginsManager.h

noinst_HEADERS = \
	tulip/Morphing.h


if WIN32 
BUILT_SOURCES = \
	tulip/TabWidgetData.h \
	tulip/PropertyDialogData.h \
	tulip/CopyPropertyDialogData.h \
	tulip/TulipStatsData.h \
	tulip/GlMainViewData.h \
	tulip/FindSelectionData.h \
	tulip/RenderingParametersDialogData.h \
	tulip/GWOverviewWidgetData.h
else
BUILT_SOURCES = \
	tulip/TabWidgetData.h \
	tulip/PropertyDialogData.h \
	tulip/CopyPropertyDialogData.h \
	tulip/TulipStatsData.h \
	tulip/FindSelectionData.h \
	tulip/RenderingParametersDialogData.h \
	tulip/GlMainViewData.h \
	tulip/GWOverviewWidgetData.h \
	tulip/sghierarchywidget.h \
	tulip/glmainwidget.h \
	tulip/layermanagerwidget.h \
	tulip/elementpropertieswidget.h \
	tulip/gwoverviewwidget.h \
	tulip/propertywidget.h

tulip/sghierarchywidget.h : tulip/SGHierarchyWidget.h
	cp -f $< $@

tulip/glmainwidget.h : tulip/GlMainWidget.h
	cp -f $< $@

tulip/layermanagerwidget.h : tulip/LayerManagerWidget.h
	cp -f $< $@

tulip/gwoverviewwidget.h : tulip/GWOverviewWidget.h
	cp -f $< $@

tulip/elementpropertieswidget.h : tulip/ElementPropertiesWidget.h
	cp -f $< $@

tulip/propertywidget.h : tulip/PropertyWidget.h
	cp -f $< $@


endif

nobase_nodist_include_HEADERS = $(BUILT_SOURCES)

CLEANFILES = $(BUILT_SOURCES)

tulip/%Data.h: ../designer/%.ui
	mkdir -p tulip
	$(UIC) -o $@ $<
