
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

add_subdirectory(spreadsheet)
add_subdirectory(parallel)


########### install files ###############




#original Makefile.am contents follow:

#SUBDIRS = spreadsheet parallel
#
