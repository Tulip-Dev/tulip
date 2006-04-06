SUBDIRS = thirdparty library plugins software docs
DIST_SUBDIRS =  thirdparty library plugins software samples docs
EXTRA_DIST = gen-conf.sh gen-doc.sh \
	     INSTALL.win \
	     $(PACKAGE)-$(VERSION).spec \
	     make_tulip_mac_bundle.sh.in \
	     tulip_mac.icns \
	     tulip_mac_info.plist.in \
	     tulip.nsi.in \
	     tulip.spec.in \
	     tulip-config.in \
	     tulip.m4

bin_SCRIPTS = tulip-config

if !WIN32
m4datadir = $(datadir)/aclocal
m4data_DATA = tulip.m4
endif 

if MACOSX
install-data-am :
	if test ! -h  $(exec_prefix)/bin/tulip.app/Contents/Info.plist; then \
		mkdir -p $(exec_prefix)/bin/tulip.app/Contents/Resources ;\
		ln -s $(abs_srcdir)/Info.plist $(exec_prefix)/bin/tulip.app/Contents/Info.plist ;\
		ln -s $(abs_srcdir)/tulip_mac.icns $(exec_prefix)/bin/tulip.app/Contents/Resources/tulip.icns ;\
	fi;

macinst: install-data-am
	sh make_tulip-$(VERSION)_mac_bundle.sh
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

if WIN32
if QT3
wininst:
	makensis.exe //DQT3 ${PACKAGE}-${VERSION}.nsi
else
wininst:
	makensis.exe ${PACKAGE}-${VERSION}.nsi
endif
endif

dist-hook:
	rm -rf 	$(PACKAGE)-$(VERSION)/thirdparty/gle/autom4te-2.53.cache
	rm -rf	$(PACKAGE)-$(VERSION)/thirdparty/gle/GL
	rm -rf 	$(PACKAGE)-$(VERSION)/autom4te-2.53.cache
	rm -f   $(PACKAGE)-$(VERSION)/$(PACKAGE)-$(VERSION).tar.gz
