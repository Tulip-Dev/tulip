UIC = @UIC@

dist_noinst_DATA = ClusterTree.ui FindSelection.ui TulipElementProperties.ui
SOURCES = TulipElementProperties.ui ClusterTree.ui FindSelection.ui elementtable.xpm GlGraphWidget.xpm clustertree.xpm
EXTRA_DIST = TulipElementProperties.ui ClusterTree.ui FindSelection.ui elementtable.xpm GlGraphWidget.xpm clustertree.xpm

classes: ../include/tulip/TulipElementPropertiesData.h ../src/TulipElementPropertiesData.cpp \
	 ../include/tulip/FindSelectionData.h ../src/FindSelectionData.cpp \
	../include/tulip/ClusterTreeData.h ../src/ClusterTreeData.cpp

all:
	${UIC} -o ../include/tulip/FindSelectionData.h FindSelection.ui
	${UIC} -o ../src/FindSelectionData.cpp -impl ../include/tulip/FindSelectionData.h FindSelection.ui

install-data-am:
	${UIC} -o ../include/tulip/FindSelectionData.h FindSelection.ui
	${UIC} -o ../src/FindSelectionData.cpp -impl ../include/tulip/FindSelectionData.h FindSelection.ui


