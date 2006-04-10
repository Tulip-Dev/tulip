SUBDIRS = thirdparty library plugins software docs
DIST_SUBDIRS =  thirdparty library plugins software samples docs
EXTRA_DIST = gen-conf.sh gen-doc.sh \
	     $(PACKAGE)-$(VERSION).spec \
	     $(PACKAGE)-$(VERSION).nsi \
	     tulip-config.in \
	     tulip.m4

bin_SCRIPTS = tulip-config

if !WIN32
m4datadir = $(datadir)/aclocal
m4data_DATA = tulip.m4
endif 

srcrpm:
	make dist
	mkdir -p RPM/BUILD RPM/RPMS RPM/SOURCES RPM/SPECS RPM/SRPMS
	echo "%_topdir	 `pwd`/RPM" > RPMMACROS
	echo "macrofiles:   `pwd`/RPMMACROS" > RPMRC
	rpmbuild --rcfile /usr/lib/rpm/rpmrc:./RPMRC \
		 -ts $(top_builddir)/$(PACKAGE)-$(VERSION).tar.gz
	mv RPM/SRPMS/*.src.rpm .
	rm -rf RPM/ RPMMACROS RPMRC $(top_builddir)/$(PACKAGE)-$(VERSION).tar.gz

wininst:
	makensis.exe ${PACKAGE}-${VERSION}.nsi

dist-hook:
	rm -rf 	$(PACKAGE)-$(VERSION)/thirdparty/gle/autom4te-2.53.cache
	rm -rf	$(PACKAGE)-$(VERSION)/thirdparty/gle/GL
	rm -rf 	$(PACKAGE)-$(VERSION)/autom4te-2.53.cache
	rm -f   $(PACKAGE)-$(VERSION)/$(PACKAGE)-$(VERSION).tar.gz
