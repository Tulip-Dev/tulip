include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR} )


########### install files ###############




#original Makefile.am contents follow:

#BUILT_SOURCES = \
#	TulipData.h\
#	AppStartUpData.h\
#	InfoDialogData.h
#
#nodist_noinst_HEADERS = $(BUILT_SOURCES)
#
#noinst_HEADERS = \
#	TulipPluginLoader.h \
#	ElementInfoToolTip.h \
#	Application.h\
#	AppStartUp.h\
#	InfoDialog.h\
#	viewGl.h
#
#CLEANFILES = $(BUILT_SOURCES)
#
#%Data.h : ../designer/%.ui
#	$(UIC) -o $@ $<
