SUBDIRS = developerHandbook userHandbook doxygen
DIST_SUBDIRS = developerHandbook userHandbook doxygen

EXTRA_DIST = gen-profile.sh common	

all : html

clean-am :
	rm -f profile.adp *~

html-am : profile.adp

install-data-am:
	cp -r common profile.adp $(pkgdatadir)/.

profile.adp : gen-profile.sh
	sh gen-profile.sh

uninstall-am:
	rm -rf $(pkgdatadir)/common $(pkgdatadir)/profile.adp


