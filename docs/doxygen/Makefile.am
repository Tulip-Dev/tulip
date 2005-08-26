EXTRA_DIST = header.html footer.html general general-html general-man tulip.html

all : html man

html: tulip-lib tulip-libogl tulip-libqt tulip-plugins tulipdoc.html profile.dcf

tulipdoc.html: ../common/gen-index.sh
	sh ../common/gen-index.sh

tulip-lib: tulip-lib.doxygen header.html footer.html 
	$(DOXYGEN) tulip-lib.doxygen

tulip-lib.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Graph Library\"" > $@
	echo "PROJECT_NUMBER = 2.0.1" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip " >> $@
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
	echo "HTML_OUTPUT = tulip-libqt">> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

tulip-plugins: tulip-plugins.doxygen header.html footer.html
	$(DOXYGEN) tulip-plugins.doxygen

tulip-plugins.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Plugins\"" > $@
	echo "PROJECT_NUMBER = 2.0.1" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/plugins " >> $@
	echo "HTML_OUTPUT = tulip-plugins">> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@
	echo "GENERATE_TAGFILE = tulip-plugins.tag" >> $@

man :  man-lib.doxygen  man-ogl.doxygen  man-qt.doxygen man-plugins.doxygen
	$(DOXYGEN) man-lib.doxygen && $(DOXYGEN) man-ogl.doxygen && $(DOXYGEN) man-qt.doxygen && $(DOXYGEN) man-plugins.doxygen
	gzip -f man/man3/*.3

man-lib.doxygen: general general-man 
	echo "PROJECT_NAME = \"Tulip Graph Library\"" > $@
	echo "PROJECT_NUMBER = 2.0.1" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip">> $@
	echo "MAN_OUTPUT = man" >> $@
	cat general-man >> $@

man-ogl.doxygen: general general-man 
	echo "PROJECT_NAME = \"Tulip Open GL Library\"" > $@
	echo "PROJECT_NUMBER = 2.0.1" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-ogl">> $@
	echo "MAN_OUTPUT = man" >> $@
	cat general-man >> $@

man-qt.doxygen: general general-man 
	echo "PROJECT_NAME = \"Tulip QT Library\"" > $@
	echo "PROJECT_NUMBER = 2.0.1" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-qt">> $@
	echo "MAN_OUTPUT = man" >> $@
	cat general-man >> $@

man-plugins.doxygen: general general-man 
	echo "PROJECT_NAME = \"Tulip Plugins Library\"" > $@
	echo "PROJECT_NUMBER = 2.0.1" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/plugins/">> $@
	echo "MAN_OUTPUT = man" >> $@
	cat general-man >> $@

profile.dcf: tulip-plugins/index.html ../common/CatalogManager.properties dox-profile.xsl
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader\
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -o profile.dcf tulip-plugins.tag  dox-profile.xsl

	
	

clean:
	rm -rf man man-lib.doxygen man-ogl.doxygen man-qt.doxygen
	rm -rf tulipdoc.html *~
	rm -rf tulip-libogl tulip-libogl.doxygen
	rm -rf tulip-libqt tulip-libqt.doxygen
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
	if test -d tulip-plugins ; then\
                cp -r tulip-plugins $(pkgdatadir)/doxygen/. ;\
        fi ;
	if test -d man ; then\
		cp -r man $(pkgdatadir)/. ;\
	fi ;

uninstall-am :
	rm -rf $(pkgdatadir)/doxygen
	rm -rf $(pkgdatadir)/man

