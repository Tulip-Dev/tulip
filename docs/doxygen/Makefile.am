EXTRA_DIST = header.html footer.html general general-html general-man tulip.html dox-html.xsl dox-profile.xsl

all : 

html: tulip-lib tulip-libogl tulip-libqt tulip-plugins tulipdoc.html profile.dcf allPlugins.html man

tulipdoc.html: ../common/gen-index.sh
	sh ../common/gen-index.sh

tulip-lib: tulip-lib.doxygen header.html footer.html 
	$(DOXYGEN) tulip-lib.doxygen

tulip-lib.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Graph Library\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip/include/tulip " >> $@
	echo "HTML_OUTPUT = tulip-lib">> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

tulip-libogl: tulip-libogl.doxygen header.html footer.html 
	$(DOXYGEN) tulip-libogl.doxygen

tulip-libogl.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Open GL Library\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-ogl/include/tulip " >> $@
	echo "HTML_OUTPUT = tulip-libogl" >> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

tulip-libqt: tulip-libqt.doxygen header.html footer.html 
	$(DOXYGEN) tulip-libqt.doxygen

tulip-libqt.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip QT Library\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-qt/include/tulip " >> $@
	echo "HTML_OUTPUT = tulip-libqt">> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

tulip-plugins: tulip-plugins.doxygen header.html footer.html
	$(DOXYGEN) tulip-plugins.doxygen

tulip-plugins.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Plugins\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/plugins " >> $@
	echo "HTML_OUTPUT = tulip-plugins">> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@
	echo "GENERATE_TAGFILE = tulip-plugins.tag" >> $@
	echo "XML_OUTPUT = xml" >> $@
	echo "GENERATE_XML = YES" >> $@

man: tulip-man

tulip-man :  man-lib.doxygen  man-ogl.doxygen  man-qt.doxygen man-plugins.doxygen
	$(DOXYGEN) man-lib.doxygen && $(DOXYGEN) man-ogl.doxygen && $(DOXYGEN) man-qt.doxygen && $(DOXYGEN) man-plugins.doxygen
	gzip -f tulip-man/man3/*.3

man-lib.doxygen: general general-man 
	echo "PROJECT_NAME = \"Tulip Graph Library\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip/include/tulip">> $@
	echo "MAN_OUTPUT = tulip-man" >> $@
	cat general-man >> $@

man-ogl.doxygen: general general-man 
	echo "PROJECT_NAME = \"Tulip Open GL Library\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-ogl/include/tulip">> $@
	echo "MAN_OUTPUT = tulip-man" >> $@
	cat general-man >> $@

man-qt.doxygen: general general-man 
	echo "PROJECT_NAME = \"Tulip QT Library\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-qt/include/tulip">> $@
	echo "MAN_OUTPUT = tulip-man" >> $@
	cat general-man >> $@

man-plugins.doxygen: general general-man 
	echo "PROJECT_NAME = \"Tulip Plugins Library\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/plugins/">> $@
	echo "MAN_OUTPUT = tulip-man" >> $@
	cat general-man >> $@

profile.dcf: tulip-plugins ../common/CatalogManager.properties dox-profile.xsl
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader\
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -o profile.dcf tulip-plugins.tag  dox-profile.xsl

allPlugins.html: tulip-plugins ../common/CatalogManager.properties dox-html.xsl
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	 com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader\
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver \
	-o allPlugins.html tulip-plugins.tag dox-html.xsl

clean:
	rm -rf tulip-man profile.dcf xml allPlugins.html
	rm -rf tulipdoc.html *~ tulip-plugins.tag *.doxygen
	rm -rf tulip-libogl tulip-libqt tulip-lib tulip-plugins

install-data-am :
	mkdir -p $(DESTDIR)$(pkgdatadir)/doxygen
	mkdir -p $(DESTDIR)$(pkgdatadir)/man
	if test -f tulipdoc.html ; then \
		cp $(srcdir)/tulip.html $(DESTDIR)$(pkgdatadir)/doxygen/. ;\
		cp tulipdoc.html $(DESTDIR)$(pkgdatadir)/doxygen/. ;\
	fi ; 
	if test -d tulip-lib ; then \
		cp -r tulip-lib $(DESTDIR)$(pkgdatadir)/doxygen/. ;\
	fi ;
	if test -d tulip-libogl ; then\
		cp -r tulip-libogl $(DESTDIR)$(pkgdatadir)/doxygen/. ;\
	fi ;
	if test -d tulip-libqt ; then\
		cp -r tulip-libqt $(DESTDIR)$(pkgdatadir)/doxygen/. ;\
	fi ;
	if test -d tulip-plugins ; then\
                cp -r tulip-plugins $(DESTDIR)$(pkgdatadir)/doxygen/. ;\
        fi ;
	if test -d tulip-man ; then\
		cp -r tulip-man/* $(DESTDIR)$(pkgdatadir)/man/. ;\
	fi ;
	if test -f allPlugins.html ; then \
		cp allPlugins.html $(DESTDIR)$(pkgdatadir)/doxygen/. ;\
	fi ;
uninstall-am :
	rm -rf $(DESTDIR)$(pkgdatadir)/doxygen
	rm -rf $(DESTDIR)$(pkgdatadir)/man

