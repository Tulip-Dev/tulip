if WIN32
MWINDOWS = -mwindows
bin_PROGRAMS = tulip tulip_app
else
bin_PROGRAMS = tulip_app tulip_need_restart
bin_SCRIPTS = tulip
tulip: tulip.sh
	cp $< $@
endif

tulip_CXXFLAGS = -DQT_THREAD_SUPPORT -D_REENTRANT
tulip_need_restart_CXXFLAGS = -DQT_THREAD_SUPPORT -D_REENTRANT
tulip_app_CXXFLAGS = -DQT_THREAD_SUPPORT -D_REENTRANT -DQT_ASSISTANT='"@QT_ASSISTANT@"'

INCLUDES = -I$(srcdir)/../include -I../include \
	   @GL_INCLUDES@ @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@ @TULIP_QT_INCLUDE@/tulip @TULIP_PLUGINSMANAGER_INCLUDE@


if MACOSX_7
tulip_LDFLAGS = -framework Carbon
tulip_app_LDFLAGS = @GL_LDFLAGS@ @LIB_GL@ `ls $(top_builddir)/library/tulip/src/*.o` `ls $(top_builddir)/thirdparty/gzstream/*.o` `ls $(top_builddir)/thirdparty/triangle/*.o` -lz  @TULIP_OGL_LDFLAGS@ `ls $(top_builddir)/library/tulip-qt/src/*.o` -framework Carbon -framework AGL
else
if MACOSX
tulip_LDFLAGS = -framework Carbon @LIB_QT@
tulip_need_restart_LDFLAGS = -framework Carbon @LIB_QT@
tulip_app_LDFLAGS = @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @TULIP_PLUGINSMANAGER_LDFLAGS@ -framework Carbon -framework AGL -lfreetype -lxml2 ${ADD_LIB} @LIB_QT_ASSISTANT@ @LIB_QT@ @LIB_IMAGES_LDFLAGS@
else
tulip_LDFLAGS = ${MWINDOWS} @TULIP_QT_LDFLAGS@ @LIB_QT@
tulip_need_restart_LDFLAGS = ${MWINDOWS} @QT_LDFLAGS@ @LIB_QT@
tulip_app_LDFLAGS = ${MWINDOWS} @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @TULIP_PLUGINSMANAGER_LDFLAGS@ -lfreetype -lxml2 ${ADD_LIB} @LIB_QT_ASSISTANT@ @LIB_QT@ @LIB_IMAGES_LDFLAGS@ ${OPENMP_CXXFLAGS}
endif
endif
AM_CPPFLAGS= @QT_CPPFLAGS@

EXTRA_DIST = CMakeLists.txt tulip.sh

if WIN32
BUILT_SOURCES = \
	TulipResource.cpp \
	moc_AppStartUp.cpp \
	moc_InfoDialog.cpp \
	moc_TulipApp.cpp \
	moc_PreferenceDialog.cpp \
	moc_ChooseControllerDialog.cpp
else
BUILT_SOURCES = \
	TulipResource.cpp \
	moc_AppStartUp.cpp \
	moc_InfoDialog.cpp \
	moc_TulipApp.cpp \
	moc_PreferenceDialog.cpp \
	moc_ChooseControllerDialog.cpp \
	tulip
endif

nodist_tulip_app_SOURCES = $(BUILT_SOURCES)

dist_tulip_SOURCES = TulipLauncher.cpp
dist_tulip_need_restart_SOURCES = TulipNeedRestart.cpp
dist_tulip_app_SOURCES = \
	TulipApp.cpp \
	Application.cpp \
	InfoDialog.cpp \
	AppStartUp.cpp \
	PreferenceDialog.cpp \
	ChooseControllerDialog.cpp \
	main.cpp

moc_%.cpp: ../include/%.h
	$(MOC) ${AM_CPPFLAGS} -o $@ $<

CLEANFILES = $(BUILT_SOURCES)

../include/%Data.h: ../designer/%.ui
	$(UIC) -o $@ $<

TulipResource.cpp: ../designer/TulipResource.qrc
	$(RCC) -o $@ $<

if MACOSX
install-data-am :
	if test ! -h  $(exec_prefix)/bin/tulip.app/Contents/@QT_ASSISTANT@.app; then \
		mkdir -p $(exec_prefix)/bin/tulip.app/Contents/ ;\
		ln -s @QT_ASSISTANT_DIR@/@QT_ASSISTANT@.app $(exec_prefix)/bin/tulip.app/Contents/@QT_ASSISTANT@.app ;\
	fi;
endif
