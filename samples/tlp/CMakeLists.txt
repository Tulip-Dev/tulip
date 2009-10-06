
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### install files ###############

install(FILES  www.labri.fr.tlp.gz groups.tlp.gz logotulip.jpg DESTINATION )



#original Makefile.am contents follow:

#noinst_DATA = www.labri.fr.tlp.gz groups.tlp.gz logotulip.jpg
