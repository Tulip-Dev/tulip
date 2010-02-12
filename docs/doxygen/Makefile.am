EXTRA_DIST = header.html footer.html general general-html general-man tulip.html \
	     dox-html.xsl dox-profile.xsl dox-plugins.xsl CMakeLists.txt

all : 

html: tulip-lib tulip-ogl-lib tulip-qt-lib tulipdoc.html profile.dcf allPluginsBody.html allPlugins.html tulipPlugins.html man

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

tulip-ogl-lib: tulip-ogl-lib.doxygen header.html footer.html 
	$(DOXYGEN) tulip-ogl-lib.doxygen

tulip-ogl-lib.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Open GL Library\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-ogl/include/tulip " >> $@
	echo "HTML_OUTPUT = tulip-ogl-lib" >> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

tulip-qt-lib: tulip-qt-lib.doxygen header.html footer.html 
	$(DOXYGEN) tulip-qt-lib.doxygen

tulip-qt-lib.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip QT Library\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-qt/include/tulip " >> $@
	echo "HTML_OUTPUT = tulip-qt-lib">> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

tulip-plugins.tag: tulip-plugins.doxygen header.html footer.html
	$(DOXYGEN) tulip-plugins.doxygen

tulip-plugins.doxygen: general
	echo "PROJECT_NAME = \"Tulip Plugins\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/plugins " >> $@
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

profile.dcf: ../common/CatalogManager.properties dox-profile.xsl tulip-plugins.tag
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar" \
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver \
	-o $@ tulip-plugins.tag  dox-profile.xsl

tulipPlugins.html: ../common/CatalogManager.properties dox-plugins.xsl tulip-plugins.tag
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar" \
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver \
	-o $@ tulip-plugins.tag  dox-plugins.xsl

allPluginsBody.html: ../common/CatalogManager.properties dox-html.xsl tulip-plugins.tag
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar" \
	 com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver \
	-o $@ tulip-plugins.tag dox-html.xsl

allPlugins.html: allPluginsBody.html 	
	cat header-allPlugins.html > $@
	cat allPluginsBody.html >> $@
	echo "</BODY>" >> $@	

clean:
	rm -rf tulip-man profile.dcf xml allPluginsBody.html allPlugins.html tulipPlugins.html
	rm -rf tulipdoc.html *~ tulip-plugins.tag *.doxygen
	rm -rf tulip-libogl tulip-libqt tulip-lib

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
	if test -d tulip-ogl-lib ; then\
		cp -r tulip-ogl-lib $(DESTDIR)$(pkgdatadir)/doxygen/. ;\
	fi ;
	if test -d tulip-qt-lib ; then\
		cp -r tulip-qt-lib $(DESTDIR)$(pkgdatadir)/doxygen/. ;\
	fi ;
	if test -d tulip-man ; then\
		cp -r tulip-man/* $(DESTDIR)$(pkgdatadir)/man/. ;\
	fi ;
	if test -f allPluginsBody.html ; then \
		cp allPluginsBody.html $(DESTDIR)$(pkgdatadir)/doxygen/. ;\
	fi ;
	if test -f allPlugins.html ; then \
		cp allPlugins.html $(DESTDIR)$(pkgdatadir)/doxygen/. ;\
	fi ;
	if test -f tulipPlugins.html ; then \
		cp tulipPlugins.html $(DESTDIR)$(pkgdatadir)/doxygen/. ;\
	fi ;
uninstall-am :
	rm -rf $(DESTDIR)$(pkgdatadir)/doxygen
	rm -rf $(DESTDIR)$(pkgdatadir)/man

