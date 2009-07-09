
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### install files ###############

install(FILES  AppStartUp.ui  		InfoDialog.ui  		Tulip.ui  		i_open.png  		i_print.png  		i_redo.png  		i_save.png  		i_undo.png  		TulipResource.qrc  		background_logo.png  		logo160x145.png  		logo32x32.png DESTINATION )



#original Makefile.am contents follow:

#dist_noinst_DATA = AppStartUp.ui \
#		InfoDialog.ui \
#		Tulip.ui \
#		i_open.png \
#		i_print.png \
#		i_redo.png \
#		i_save.png \
#		i_undo.png \
#		TulipResource.qrc \
#		background_logo.png \
#		logo160x145.png \
#		logo32x32.png
