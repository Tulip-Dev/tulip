nobase_include_HEADERS = \
	tulip/TulipTableWidget.h \
	tulip/GlGraphWidget.h \
	tulip/GWInteractor.h \
	tulip/MouseBoxZoomer.h\
	tulip/MouseEdgeBuilder.h\
	tulip/MouseInteractors.h \
	tulip/MouseMagicSelector.h\
	tulip/MouseNodeBuilder.h\
	tulip/MouseSelectionEditor.h\
	tulip/MouseSelector.h\
	tulip/MouseEdgeSelector.h \
	tulip/MouseEdgeBendEditor.h \
	tulip/PropertyWidget.h \
	tulip/ElementPropertiesWidgetUI.h \
	tulip/ElementPropertiesWidget.h \
	tulip/FindSelectionWidget.h \
	tulip/SGHierarchyWidgetUI.h \
	tulip/SGHierarchyWidget.h \
	tulip/GWOverviewWidget.h \
	tulip/LayerManagerWidget.h \
	tulip/LayerManagerWidgetData.h \
	tulip/Qt3ForTulip.h \
	tulip/TlpQtTools.h \
	tulip/SGHierarchyWidgetPlugin.h \
	tulip/SGHierarchyWidgetQt3.h \
	tulip/GlGraphWidgetPlugin.h \
	tulip/ElementPropertiesWidgetPlugin.h \
	tulip/GWOverviewWidgetPlugin.h \
	tulip/PropertyWidgetPlugin.h

noinst_HEADERS = \
	tulip/Morphing.h


if WIN32 
BUILT_SOURCES = \
	tulip/FindSelectionData.h \
	tulip/RenderingParametersDialogData.h \
	tulip/GWOverviewWidgetData.h 
else
BUILT_SOURCES = \
	tulip/FindSelectionData.h \
	tulip/RenderingParametersDialogData.h \
	tulip/GWOverviewWidgetData.h \
	tulip/sghierarchywidget.h \
	tulip/glgraphwidget.h \
	tulip/layermanagerwidget.h \
	tulip/elementpropertieswidget.h \
	tulip/gwoverviewwidget.h \
	tulip/propertywidget.h

tulip/sghierarchywidget.h : tulip/SGHierarchyWidget.h
	cp -f $< $@

tulip/glgraphwidget.h : tulip/GlGraphWidget.h
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
