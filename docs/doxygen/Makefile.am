EXTRA_DIST = header.html footer.html general general-html general-man tulip.html

all : html man

html: tulip-lib tulip-libogl tulip-libqt tulipdoc.html

tulipdoc.html: ../common/gen-index.sh
	sh ../common/gen-index.sh

tulip-lib: tulip-lib.doxygen header.html footer.html 
	$(DOXYGEN) tulip-lib.doxygen

tulip-lib.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Graph Library\"" > $@
	echo "PROJECT_NUMBER = 2.0.1" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip " >> $@
	echo "FILE_PATTERNS =  $(TULIP_LIB)" >> $@
	echo "HTML_OUTPUT = tulip-lib">> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

tulip-libogl: tulip-libogl.doxygen header.html footer.html 
	$(DOXYGEN) tulip-libogl.doxygen

tulip-libogl.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Open GL Library\"" > $@
	echo "PROJECT_NUMBER = 2.0.1" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-ogl " >> $@
	echo "FILE_PATTERNS =  $(TULIP_OGL)" >> $@
	echo "HTML_OUTPUT = tulip-libogl" >> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

tulip-libqt: tulip-libqt.doxygen header.html footer.html 
	$(DOXYGEN) tulip-libqt.doxygen

tulip-libqt.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip QT Library\"" > $@
	echo "PROJECT_NUMBER = 2.0.1" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-qt " >> $@
	echo "FILe_PATTERNS = $(TULIP_QT)">> $@
	echo "HTML_OUTPUT = tulip-libqt">> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

man :  man-lib.doxygen  man-ogl.doxygen  man-qt.doxygen
	$(DOXYGEN) man-lib.doxygen && $(DOXYGEN) man-ogl.doxygen && $(DOXYGEN) man-qt.doxygen
	gzip -f man/man3/*.3

man-lib.doxygen: general general-man 
	echo "PROJECT_NAME = \"Tulip Graph Library\"" > $@
	echo "PROJECT_NUMBER = 2.0.1" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip">> $@
	echo "MAN_OUTPUT = man" >> $@
	echo "FILE_PATTERNS = $(TULIP_LIB)" >> $@
	cat general-man >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

man-ogl.doxygen: general general-man 
	echo "PROJECT_NAME = \"Tulip Open GL Library\"" > $@
	echo "PROJECT_NUMBER = 2.0.1" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-ogl">> $@
	echo "MAN_OUTPUT = man" >> $@
	echo "FILE_PATTERNS = $(TULIP_OGL)" >> $@
	cat general-man >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

man-qt.doxygen: general general-man 
	echo "PROJECT_NAME = \"Tulip QT Library\"" > $@
	echo "PROJECT_NUMBER = 2.0.1" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-qt">> $@
	echo "MAN_OUTPUT = man" >> $@
	echo "FILE_PATTERNS = $(TULIP_QT)" >> $@
	cat general-man >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

clean:
	echo "DOT_PATH = "`dirname $(DOT)`
	rm -rf man man-lib.doxygen man-ogl.doxygen man-qt.doxygen
	rm -rf tulipdoc.html *~
	rm -rf tulip-libogl tulip-libogl.doxygen
	rm -rf tulip-libqt tulip-libqt.doxygen
	rm -rf tulip-soft tulip-soft.doxygen
	rm -rf tulip-lib tulip-lib.doxygen
	rm -rf tulip-plugins tulip-plugins.doxygen

install-data-am :
	mkdir -p $(pkgdatadir)/doxygen
	cp tulip.html $(pkgdatadir)/doxygen/.
	if test -f tulipdoc.html ; then \
		cp tulipdoc.html $(pkgdatadir)/doxygen/. ;\
	fi ; 
	if test -d tulip-lib ; then \
		cp -r tulip-lib $(pkgdatadir)/doxygen/. ;\
	fi ;
	if test -d tulip-libogl ; then\
		cp -r tulip-libogl $(pkgdatadir)/doxygen/. ;\
	fi ;
	if test -d tulip-libqt ; then\
		cp -r tulip-libqt $(pkgdatadir)/doxygen/. ;\
	fi ;
	if test -d man ; then\
		cp -r man $(pkgdatadir)/. ;\
	fi ;

uninstall-am :
	rm -rf $(pkgdatadir)/doxygen
	rm -rf $(pkgdatadir)/man

include Headers.am
