nobase_include_HEADERS = \
	tulip/TulipTableWidget.h \
	tulip/GlGraphWidget.h \
	tulip/MouseObject.h \
	tulip/MouseZoomBox.h \
	tulip/MouseInterface.h \
	tulip/PropertyWidgets.h \
	tulip/TulipElementPropertiesData.h \
	tulip/TulipElementProperties.h \
	tulip/FindSelection.h \
	tulip/ClusterTreeData.h \
	tulip/ClusterTree.h \
	tulip/MouseAddEdge.h\
	tulip/MouseAddNode.h\
	tulip/MouseMoveSelection.h\
	tulip/MouseSelection.h\
	tulip/MouseMagicSelection.h\
	tulip/Morphing.h \
	tulip/Qt3ForTulip.h \
	tulip/TlpQtTools.h \
	tulip/ClusterTreePlugin.h \
	tulip/ClusterTreeQt3.h \
	tulip/GlGraphWidgetPlugin.h \
	tulip/TulipElementPropertiesPlugin.h \
	tulip/TulipPropertyTablePlugin.h

if QT3
nobase_nodist_include_HEADERS = tulip/FindSelectionData.h
else
nobase_nodist_include_HEADERS = \
	tulip/FindSelectionData.h \
	tulip/clustertree.h \
	tulip/glgraphwidget.h \
	tulip/tulipelementproperties.h \
	tulip/tulippropertytable.h

tulip/clustertree.h : tulip/ClusterTree.h
	cp -f $< $@

tulip/glgraphwidget.h : tulip/GlGraphWidget.h
	cp -f $< $@

tulip/tulipelementproperties.h : tulip/TulipElementProperties.h
	cp -f $< $@

tulip/tulippropertytable.h : tulip/PropertyWidgets.h
	cp -f $< $@
endif

tulip/FindSelectionData.h: ../designer/FindSelection.ui
	$(UIC) -o $@ $<
