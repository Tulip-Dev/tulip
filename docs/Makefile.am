SUBDIRS = developerHandbook userHandbook doxygen devTeamDoxygen
DIST_SUBDIRS = developerHandbook userHandbook doxygen devTeamDoxygen

EXTRA_DIST = gen-profile.sh common	

all :

clean-am :
	rm -f profile.adp *~ common/*~

html-am :
	sh gen-profile.sh
install-data-am:
	mkdir -p $(pkgdatadir)/common
	cp -r common/*.png common/*.css common/favicon.ico $(pkgdatadir)/common/.
	if test -f profile.adp ; then \
		cp profile.adp $(pkgdatadir)/.; \
	fi;

uninstall-am:
	rm -rf $(pkgdatadir)/common $(pkgdatadir)/profile.adp


