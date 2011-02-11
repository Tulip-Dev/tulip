
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

add_subdirectory(common)
add_subdirectory(doxygen)
add_subdirectory(userHandbook)
add_subdirectory(developerHandbook)

ADD_CUSTOM_TARGET(tulip_qhcp ALL ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/tulip.qhcp ${CMAKE_CURRENT_BINARY_DIR}/tulip.qhcp)

ADD_CUSTOM_TARGET(tulip_qhp ALL sh ${PROJECT_SOURCE_DIR}/docs/gen-tulipqhp.sh ${CMAKE_CURRENT_BINARY_DIR}/tulip.qhp)

ADD_DEPENDENCIES(tulip_qhp common_copy dev_toc dev_keywords dox_toc dox_keywords user_toc user_keywords)

ADD_CUSTOM_TARGET(tulip_qch ALL qhelpgenerator ${CMAKE_CURRENT_BINARY_DIR}/tulip.qhp -o ${CMAKE_CURRENT_BINARY_DIR}/tulip.qch)

ADD_DEPENDENCIES(tulip_qch tulip_qhp)

ADD_CUSTOM_TARGET(tulip_qhc ALL qcollectiongenerator ${CMAKE_CURRENT_BINARY_DIR}/tulip.qhcp -o ${CMAKE_CURRENT_BINARY_DIR}/tulip.qhc WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}) 

ADD_DEPENDENCIES(tulip_qhc tulip_qch tulip_qhcp)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tulip.qhc
  DESTINATION share/tulip/
  COMPONENT tulip_doc)


#original Makefile.am contents follow:

#SUBDIRS = developerHandbook userHandbook doxygen devTeamDoxygen
#DIST_SUBDIRS = developerHandbook userHandbook doxygen devTeamDoxygen
#
#EXTRA_DIST = gen-profile.sh \
#	common/*.png \
#	common/*.gif \
#	common/*.css \
#	common/*.xsl \
#	common/favicon.ico \
#	common/CatalogManager.properties \
#	common/gen-index.sh \
#	common/dtd/*.dtd \
#	common/dtd/*.mod \
#	common/dtd/README \
#	common/dtd/docbook-xml-4.4.zip \
#	common/dtd/docbook.cat \
#	common/dtd/ChangeLog \
#	common/dtd/catalog.xml \
#	common/dtd/ent/*.ent \
#	common/dtd/ent/README \
#	README
#
#all :
#
#clean-am :
#	rm -f profile.adp *~ common/*~
#
#html-am :
#	sh gen-profile.sh
#
#install-data-am :
#if MACOSX
#	if test ! -h  $(DESTDIR)$(exec_prefix)/bin/tulip.app/Contents/share/tulip; then \
#		mkdir -p $(DESTDIR)$(exec_prefix)/bin/tulip.app/Contents/share ;\
#		ln -s $(pkgdatadir) $(DESTDIR)$(exec_prefix)/bin/tulip.app/Contents/share/tulip ;\
#	fi;
#endif
#	if test -f profile.adp ; then \
#		cp profile.adp $(DESTDIR)$(pkgdatadir)/.; \
#		mkdir -p $(DESTDIR)$(pkgdatadir)/common ;\
#		cp -r $(srcdir)/common/*.png $(srcdir)/common/*.gif $(srcdir)/common/*.css $(srcdir)/common/favicon.ico $(DESTDIR)$(pkgdatadir)/common/. ;\
#	fi;
#
#uninstall-am:
#	rm -rf $(DESTDIR)$(pkgdatadir)/common $(DESTDIR)$(pkgdatadir)/profile.adp
#
#
