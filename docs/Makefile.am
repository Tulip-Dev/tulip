SUBDIRS = developerHandbook userHandbook doxygen devTeamDoxygen
DIST_SUBDIRS = developerHandbook userHandbook doxygen devTeamDoxygen

EXTRA_DIST = gen-profile.sh common	

all : html

clean-am :
	rm -f profile.adp *~ common/*~

html-am : profile.adp

install-data-am:
	mkdir -p $(pkgdatadir)/common
	cp -r common/*.png common/*.css common/favicon.ico $(pkgdatadir)/common/.
	cp profile.adp $(pkgdatadir)/.

profile.adp : gen-profile.sh
	sh gen-profile.sh

uninstall-am:
	rm -rf $(pkgdatadir)/common $(pkgdatadir)/profile.adp


