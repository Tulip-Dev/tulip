if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

EXTRA_DIST = CMakeLists.txt

plugins_LTLIBRARIES = libSpreadSheet-@VERSION@.la

pluginsdir = $(libdir)/tlp/view

libSpreadSheet_@VERSION@_la_SOURCES = \
			SpreadView.h \
			SpreadView.cpp \
			SpreadWidget.cpp \
			SpreadTable.cpp \
			SpreadCell.cpp \
			SpreadCalculator.cpp \
			SpreadValue.cpp \
			SpreadItemDelegate.cpp \
			SpreadCommands.cpp \
			SpreadConfigurationWidget.cpp \
			SpreadWidget.h \
			SpreadTable.h \
			SpreadCell.h \
			SpreadCalculator.h \
			SpreadValue.h \
			SpreadItemDelegate.h \
			SpreadCommands.h \
			SpreadLineEdit.h \
			SpreadConfigurationWidget.h

nodist_libSpreadSheet_@VERSION@_la_SOURCES = \
			moc_SpreadView.cpp \
			moc_SpreadWidget.cpp \
			moc_SpreadTable.cpp \
			moc_SpreadCell.cpp \
			moc_SpreadCalculator.cpp \
			moc_SpreadValue.cpp \
			moc_SpreadItemDelegate.cpp \
			moc_SpreadCommands.cpp \
			moc_SpreadLineEdit.cpp \
			moc_SpreadConfigurationWidget.cpp 

BUILT_SOURCES = SpreadInsertColumnDialogData.h SpreadMainWidgetData.h SpreadConfigurationWidgetData.h
 
dist_noinst_DATA = SpreadInsertColumnDialog.ui SpreadMainWidget.ui SpreadConfigurationWidget.ui

nobase_nodist_include_HEADERS = $(BUILT_SOURCES)

AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@
AM_CPPFLAGS= @QT_CPPFLAGS@
INCLUDES = @TULIP_INCLUDE@ @TULIP_QT_INCLUDE@ @TULIP_OGL_INCLUDE@ @GL_INCLUDES@ @QT_INCLUDES@

moc_%.cpp: %.h 
	$(MOC) -o $@ $<

%Data.h: %.ui
	$(UIC) -o $@ $<

clean-generic:
	rm -f moc_* *Data.*


