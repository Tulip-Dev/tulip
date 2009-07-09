
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

add_subdirectory(clustering)
add_subdirectory(colors)
add_subdirectory(import)
add_subdirectory(layout)
add_subdirectory(metric)
add_subdirectory(selection)
add_subdirectory(sizes)
add_subdirectory(string)
add_subdirectory(int)
add_subdirectory(export)
add_subdirectory(glyph)
add_subdirectory(controller)
add_subdirectory(view)
add_subdirectory(interactor)


########### install files ###############




#original Makefile.am contents follow:

#SUBDIRS = clustering  colors  import  layout  metric  selection  sizes  string int export glyph controller view interactor 
#EXTRA_DIST = mainpage.dox
#
#if MACOSX
#install-data-am :
#	if test ! -h  $(exec_prefix)/bin/tulip.app/Contents/lib/tlp; then \
#		mkdir -p $(exec_prefix)/bin/tulip.app/Contents/lib ;\
#		ln -s $(exec_prefix)/lib/tlp $(exec_prefix)/bin/tulip.app/Contents/lib/tlp ;\
#	fi;
#endif
