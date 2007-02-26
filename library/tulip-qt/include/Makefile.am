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
	tulip/PropertyWidget.h \
	tulip/ElementPropertiesWidgetData.h \
	tulip/ElementPropertiesWidget.h \
	tulip/FindSelection.h \
	tulip/SGHierarchyWidgetData.h \
	tulip/SGHierarchyWidget.h \
	tulip/GWOverviewWidget.h \
	tulip/Qt3ForTulip.h \
	tulip/TlpQtTools.h \
	tulip/SGHierarchyWidgetPlugin.h \
	tulip/SGHierarchyWidgetQt3.h \
	tulip/GlGraphWidgetPlugin.h \
	tulip/ElementPropertiesWidgetPlugin.h \
	tulip/PropertyWidgetPlugin.h

noinst_HEADERS = \
	tulip/Morphing.h


if QT3
nobase_nodist_include_HEADERS = tulip/FindSelectionData.h \
				tulip/GWOverviewWidgetData.h
else
nobase_nodist_include_HEADERS = \
	tulip/Morphing.h \
	tulip/FindSelectionData.h \
	tulip/GWOverviewWidgetData.h \
	tulip/sghierarchywidget.h \
	tulip/glgraphwidget.h \
	tulip/elementpropertieswidget.h \
	tulip/gwoverviewwidget.h \
	tulip/propertywidget.h

tulip/sghierarchywidget.h : tulip/SGHierarchyWidget.h
	cp -f $< $@

tulip/glgraphwidget.h : tulip/GlGraphWidget.h
	cp -f $< $@

tulip/gwoverviewwidget.h : tulip/GWOverviewWidget.h
	cp -f $< $@

tulip/elementpropertieswidget.h : tulip/ElementPropertiesWidget.h
	cp -f $< $@

tulip/propertywidget.h : tulip/PropertyWidget.h
	cp -f $< $@
endif

tulip/FindSelectionData.h: ../designer/FindSelection.ui
	mkdir -p tulip
	$(UIC) -o $@ $<

tulip/GWOverviewWidgetData.h: ../designer/GWOverviewWidget.ui
	mkdir -p tulip
	$(UIC) -o $@ $<
