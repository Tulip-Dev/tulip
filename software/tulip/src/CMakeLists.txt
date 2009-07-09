FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

MACRO (TULIP_QT4_WRAP_UI outfiles )
  QT4_EXTRACT_OPTIONS(ui_files ui_options ${ARGN})
  FOREACH (it ${ui_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/../include/${outfile}Data.h)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_UIC_EXECUTABLE}
      ARGS ${ui_options} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH (it)
ENDMACRO (TULIP_QT4_WRAP_UI)

MACRO (TULIP_QT4_WRAP_CPP outfiles )
  QT4_GET_MOC_FLAGS(moc_flags)
  QT4_EXTRACT_OPTIONS(moc_files moc_options ${ARGN})

  FOREACH (it ${moc_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(it ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/moc_${outfile}.cpp)
    QT4_CREATE_MOC_COMMAND(${it} ${outfile} "${moc_flags}" "${moc_options}")
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH(it)

ENDMACRO (TULIP_QT4_WRAP_CPP)	

set(tulip_UI
    ../designer/Tulip.ui
    ../designer/AppStartUp.ui
    ../designer/InfoDialog.ui)

set(tulip_MOC
    ../include/AppStartUp.h
    ../include/InfoDialog.h
    ../include/viewGl.h)

set(tulip_SRCS 
    viewGl.cpp
    Application.cpp
    geometrie2D.cpp
    InfoDialog.cpp
    AppStartUp.cpp
    main.cpp)

TULIP_QT4_WRAP_UI(tulip_UI_SRCS ${tulip_UI})
TULIP_QT4_WRAP_CPP(tulip_MOC_SRCS ${tulip_MOC})

INCLUDE_DIRECTORIES(../include/ ${TULIP_INCLUDE} ${TULIP_QT_INCLUDE} ${TULIP_OGL_INCLUDE} ${TULIP_PLUGINSMANAGER_INCLUDE})

ADD_EXECUTABLE( tulip-exe ${tulip_UI_SRCS} ${tulip_MOC_SRCS} ${tulip_SRCS} )
set_target_properties(tulip-exe PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}${TULIP_LIB_INSTALL_DIR}")
TARGET_LINK_LIBRARIES( tulip-exe ${FREETYPE_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe ${GZSTREAM_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe ${QT_LIBRARIES} )
TARGET_LINK_LIBRARIES( tulip-exe ${QT_QTASSISTANTCLIENT_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe ${QT_QTXML_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe ${QT_QTOPENGL_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe ${OPENGL_glu_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe ${OPENGL_gl_LIBRARY} )
TARGET_LINK_LIBRARIES( tulip-exe ${LIBXML2_LIBRARIES} )
TARGET_LINK_LIBRARIES( tulip-exe tulip )
TARGET_LINK_LIBRARIES( tulip-exe tulip-ogl )
TARGET_LINK_LIBRARIES( tulip-exe tulip-qt )
TARGET_LINK_LIBRARIES( tulip-exe tulip-pluginsmanager )

########### install files ###############

INSTALL_TARGETS(${TULIP_BIN_INSTALL_DIR} tulip-exe)


#original Makefile.am contents follow:

#if WIN32
#MWINDOWS = -mwindows
#endif
#
#bin_PROGRAMS = tulip
#
#tulip_CXXFLAGS = -DQT_THREAD_SUPPORT -D_REENTRANT -DQT_ASSISTANT='"@QT_ASSISTANT@"'
#
#INCLUDES = -I$(srcdir)/../include -I../include \
#	   @GL_INCLUDES@ @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@ @TULIP_QT_INCLUDE@/tulip @TULIP_PLUGINSMANAGER_INCLUDE@
#
#
#if MACOSX_7
#AM_LDFLAGS = @GL_LDFLAGS@ @LIB_GL@ `ls $(top_builddir)/library/tulip/src/*.o` `ls $(top_builddir)/thirdparty/gzstream/*.o` `ls $(top_builddir)/thirdparty/triangle/*.o` -lz  @TULIP_OGL_LDFLAGS@ `ls $(top_builddir)/library/tulip-qt/src/*.o` -framework Carbon -framework AGL
#else
#if MACOSX
#AM_LDFLAGS = @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @TULIP_PLUGINSMANAGER_LDFLAGS@ -framework Carbon -framework AGL -lfreetype -lxml2 ${ADD_LIB} @LIB_QT_ASSISTANT@ @LIB_QT@ @LIB_IMAGES_LDFLAGS@
#else
#AM_LDFLAGS = ${MWINDOWS} @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @TULIP_PLUGINSMANAGER_LDFLAGS@ -lfreetype -lxml2 ${ADD_LIB} @LIB_QT_ASSISTANT@ @LIB_QT@ @LIB_IMAGES_LDFLAGS@
#endif
#endif
#AM_CPPFLAGS= @QT_CPPFLAGS@
#
#BUILT_SOURCES = \
#	TulipResource.cpp \
#	moc_AppStartUp.cpp \
#	moc_InfoDialog.cpp \
#	moc_viewGl.cpp
#
#nodist_tulip_SOURCES = $(BUILT_SOURCES)
#
#dist_tulip_SOURCES = \
#	viewGl.cpp \
#	Application.cpp \
#	geometrie2D.cpp \
#	InfoDialog.cpp \
#	AppStartUp.cpp \
#	main.cpp
#
#moc_%.cpp: ../include/%.h
#	$(MOC) ${AM_CPPFLAGS} -o $@ $<
#
#CLEANFILES = $(BUILT_SOURCES)
#
#../include/%Data.h: ../designer/%.ui
#	$(UIC) -o $@ $<
#
#TulipResource.cpp: ../designer/TulipResource.qrc
#	$(RCC) -o $@ $<
#
#if MACOSX
#install-data-am :
#	if test ! -d  $(exec_prefix)/bin/tulip.app/Contents/@QT_ASSISTANT@.app; then \
#		mkdir -p $(exec_prefix)/bin/tulip.app/Contents/ ;\
#		ln -s @QT_ASSISTANT_DIR@/@QT_ASSISTANT@.app $(exec_prefix)/bin/tulip.app/Contents/@QT_ASSISTANT@.app ;\
#	fi;
#endif
