EXTRA_DIST = *.docbook \
	images/*.png

DOCBOOK_SRC = index.docbook \
	data_structure.docbook \
	doc_guides.docbook \
	prog_guides.docbook \
	intro_install.docbook \
	tulip_libogl.docbook \
	tulip_libqt.docbook \
	graph_format.docbook \
	plugins.docbook

install-data-am :
	if test -d html ; then \
	  mkdir -p $(DESTDIR)$(pkgdatadir)/developerHandbook ;\
	  cp -r images $(DESTDIR)$(pkgdatadir)/developerHandbook ;\
	  cp -r html $(DESTDIR)$(pkgdatadir)/developerHandbook ;\
	fi;

uninstall-am :
	rm -rf $(DESTDIR)$(pkgdatadir)/developerHandbook

all :

check :
	$(XMLLINT) --valid -noout $(srcdir)/index.docbook

clean :
	rm -rf html pdf profile.dcf *~

html : html/index.html profile.dcf

html/index.html : $(DOCBOOK_SRC) $(srcdir)/../common/tulip-html.xsl $(srcdir)/../common/CatalogManager.properties
	$(JAVA) -cp "$(top_srcdir)/thirdparty/saxon/saxon653.jar:$(top_srcdir)/thirdparty/saxon/saxon.jar:$(srcdir)/../common:$(top_srcdir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u \
	$(srcdir)/index.docbook  $(srcdir)/../common/tulip-html.xsl

pdf :   pdf/devHandbook.fo pdf/devHandbook.pdf

pdf/devHandbook.fo : $(DOCBOOK_SRC) $(srcdir)/../common/tulip-pdf.xsl $(srcdir)/../common/CatalogManager.properties
	$(JAVA) -cp "$(top_srcdir)/thirdparty/saxon/saxon653.jar:$(top_srcdir)/thirdparty/saxon/saxon.jar:$(srcdir)/../common:$(top_srcdir)/thirdparty/xml-commons/resolver.jar" \
	com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader\
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u\
	-o pdf/devHandbook.fo $(srcdir)/index.docbook  $(srcdir)/../common/tulip-pdf.xsl

pdf/devHandbook.pdf : pdf/devHandbook.fo
	$(PDFXMLTEX) --output-directory="pdf/" pdf/devHandbook.fo && \
	$(PDFXMLTEX) --output-directory="pdf/" pdf/devHandbook.fo


profile.dcf : $(srcdir)/../common/tulip-profile.xsl $(DOCBOOK_SRC) $(srcdir)/../common/CatalogManager.properties
	$(JAVA) -cp "$(top_srcdir)/thirdparty/saxon/saxon653.jar:$(top_srcdir)/thirdparty/saxon/saxon.jar:$(srcdir)/../common:$(top_srcdir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader\
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u\
	-o profile.dcf $(srcdir)/index.docbook  $(srcdir)/../common/tulip-profile.xsl install.path="developerHandbook/html/"

