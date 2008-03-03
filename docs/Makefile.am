SUBDIRS = developerHandbook userHandbook doxygen devTeamDoxygen
DIST_SUBDIRS = developerHandbook userHandbook doxygen devTeamDoxygen

EXTRA_DIST = gen-profile.sh \
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
	README

all :

clean-am :
	rm -f profile.adp *~ common/*~

html-am :
	sh gen-profile.sh

install-data-am :
if MACOSX
	if test ! -h  $(DESTDIR)$(exec_prefix)/bin/tulip.app/Contents/share/tulip; then \
		mkdir -p $(DESTDIR)$(exec_prefix)/bin/tulip.app/Contents/share ;\
		ln -s $(pkgdatadir) $(DESTDIR)$(exec_prefix)/bin/tulip.app/Contents/share/tulip ;\
	fi;
endif
	if test -f profile.adp ; then \
		cp profile.adp $(DESTDIR)$(pkgdatadir)/.; \
		mkdir -p $(DESTDIR)$(pkgdatadir)/common ;\
		cp -r $(srcdir)/common/*.png $(srcdir)/common/*.gif $(srcdir)/common/*.css $(srcdir)/common/favicon.ico $(DESTDIR)$(pkgdatadir)/common/. ;\
	fi;

uninstall-am:
	rm -rf $(DESTDIR)$(pkgdatadir)/common $(DESTDIR)$(pkgdatadir)/profile.adp


