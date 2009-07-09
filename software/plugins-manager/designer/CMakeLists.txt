
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### install files ###############

install(FILES  ProxyConfigurationDialog.ui DESTINATION )



#original Makefile.am contents follow:

#dist_noinst_DATA = ProxyConfigurationDialog.ui
#
#SOURCES = ProxyConfigurationDialog.ui
#
#EXTRA_DIST = $(SOURCES)
