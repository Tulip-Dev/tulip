SUBDIRS = developerHandbook userHandbook doxygen devTeamDoxygen
DIST_SUBDIRS = developerHandbook userHandbook doxygen devTeamDoxygen

EXTRA_DIST = gen-tulipqhp.sh \
	common/*.png \
	common/*.gif \
	common/*.css \
	common/*.xsl \
	common/favicon.ico \
	common/CatalogManager.properties \
	common/gen-index.sh \
	common/dtd/*.dtd \
	common/dtd/*.mod \
	common/dtd/README \
	common/dtd/docbook-xml-4.4.zip \
	common/dtd/docbook.cat \
	common/dtd/ChangeLog \
	common/dtd/catalog.xml \
	common/dtd/ent/*.ent \
	common/dtd/ent/README \
	README \
	common/CMakeLists.txt \
	tulip.qhcp \
	CMakeLists.txt

all :

clean-am :
	rm -f tulip.qhc tulip.qhp tulip.qch *~ common/*~

html-am : tulip.qhc

tulip.qhc : tulip.qch tulip.qhcp
	qcollectiongenerator tulip.qhcp -o $@

tulip.qch : tulip.qhp
	qhelpgenerator tulip.qhp -o $@

tulip.qhp : 
	sh gen-tulipqhp.sh

install-data-am :
if MACOSX
	if test ! -h  $(DESTDIR)$(exec_prefix)/bin/tulip.app/Contents/share/tulip; then \
		mkdir -p $(DESTDIR)$(exec_prefix)/bin/tulip.app/Contents/share ;\
		ln -s $(pkgdatadir) $(DESTDIR)$(exec_prefix)/bin/tulip.app/Contents/share/tulip ;\
	fi;
endif
	if test -f tulip.qhc ; then \
		cp tulip.qhc $(DESTDIR)$(pkgdatadir)/.; \
	fi;

uninstall-am:
	rm -rf $(DESTDIR)$(pkgdatadir)/common $(DESTDIR)$(pkgdatadir)/tulip.qhc


