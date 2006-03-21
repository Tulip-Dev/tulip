SUBDIRS = clustering  colors  import  layout  metric  selection  sizes  string int export glyph
EXTRA_DIST = mainpage.dox

if MACOSX
install-data-am :
	if test ! -h  $(exec_prefix)/bin/tulip.app/Contents/lib/tlp; then \
		mkdir -p $(exec_prefix)/bin/tulip.app/Contents/lib ;\
		ln -s $(exec_prefix)/lib/tlp $(exec_prefix)/bin/tulip.app/Contents/lib/tlp ;\
	fi;
endif
