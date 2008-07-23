if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

plugins_LTLIBRARIES = libSpreadSheet-@VERSION@.la

pluginsdir = $(libdir)/tlp/view

libSpreadSheet_@VERSION@_la_SOURCES = \
			SpreadWidget.cpp \
			SpreadTable.cpp \
			SpreadCell.cpp \
			SpreadCalculator.cpp \
			SpreadValue.cpp \
			SpreadItemDelegate.cpp \
			SpreadCommands.cpp \
			SpreadWidget.h \
			SpreadTable.h \
			SpreadCell.h \
			SpreadCalculator.h \
			SpreadValue.h \
			SpreadItemDelegate.h \
			SpreadCommands.h \
			SpreadLineEdit.h

nodist_libSpreadSheet_@VERSION@_la_SOURCES = \
			moc_SpreadWidget.cpp \
			moc_SpreadTable.cpp \
			moc_SpreadCell.cpp \
			moc_SpreadCalculator.cpp \
			moc_SpreadValue.cpp \
			moc_SpreadItemDelegate.cpp \
			moc_SpreadCommands.cpp \
			moc_SpreadLineEdit.cpp

BUILT_SOURCES = \
			SpreadInsertColumnDialogData.h\
			SpreadMainWidgetData.h


nobase_nodist_include_HEADERS = $(BUILT_SOURCES)

AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_QT_LDFLAGS@ @TULIP_OGL_LDFLAGS@
AM_CPPFLAGS= @QT_CPPFLAGS@
INCLUDES = @TULIP_INCLUDE@ @TULIP_QT_INCLUDE@ @TULIP_OGL_INCLUDE@ @GL_INCLUDES@ @LIBGLE_INCLUDE@

moc_%.cpp: %.h 
	$(MOC) -o $@ $<

%Data.h: %.ui
	$(UIC) -o $@ $<

clean-generic:
	rm -f moc_* *Data.*


