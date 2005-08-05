SUBDIRS = developerHandbook userHandbook doxygen
DIST_SUBDIRS = developerHandbook userHandbook doxygen

EXTRA_DIST = gen-profile.sh \
	common	

nobase_pkgdata_DATA = common/1.png \
	common/2.png \
	common/3.png \
	common/4.png \
	common/5.png \
	common/6.png \
	common/7.png \
	common/8.png \
	common/9.png \
	common/10.png \
	common/CatalogManager.properties \
	common/favicon.ico \
	common/logo_bg.png \
	common/logo-icon.png \
	common/logo.png \
	common/tulip-default.css \
	common/tulip-doxygen.css \
	profile.adp

all : html

html-am : profile.adp

profile.adp : gen-profile.sh
	sh gen-profile.sh

