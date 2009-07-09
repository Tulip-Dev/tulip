
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

add_subdirectory(gml)
add_subdirectory(matrix)
add_subdirectory(tlp)


########### install files ###############




#original Makefile.am contents follow:

#SUBDIRS = gml matrix tlp
