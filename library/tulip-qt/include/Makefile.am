if QT3
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
	tulip/FindSelectionData.h \
	tulip/ClusterTreeData.h \
	tulip/ClusterTree.h \
	tulip/MouseAddEdge.h\
	tulip/MouseAddNode.h\
	tulip/MouseMoveSelection.h\
	tulip/MouseSelection.h\
	tulip/MouseMagicSelection.h\
	tulip/MouseTreeFishEyes.h\
	tulip/Morphing.h \
	tulip/TlpQtTools.h
else
nobase_include_HEADERS = \
	tulip/TulipTableWidget.h \
	tulip/GlGraphWidget.h \
	tulip/glgraphwidget.h \
	tulip/MouseObject.h \
	tulip/MouseZoomBox.h \
	tulip/MouseInterface.h \
	tulip/PropertyWidgets.h \
	tulip/tulippropertytable.h \
	tulip/TulipElementPropertiesData.h \
	tulip/TulipElementProperties.h \
	tulip/tulipelementproperties.h \
	tulip/FindSelection.h \
	tulip/FindSelectionData.h \
	tulip/ClusterTreeData.h \
	tulip/ClusterTree.h \
	tulip/clustertree.h \
	tulip/MouseAddEdge.h\
	tulip/MouseAddNode.h\
	tulip/MouseMoveSelection.h\
	tulip/MouseSelection.h\
	tulip/MouseMagicSelection.h\
	tulip/Morphing.h \
	tulip/Qt3ForTulip.h \
	tulip/TlpQtTools.h

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
