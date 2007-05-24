EXTRA_DIST = *.docbook images/*.png

DOCBOOK_SRC = index.docbook functions.docbook graph_interface.docbook tutorials.docbook

install-data-am :
	if test -d html ; then \
	  mkdir -p $(DESTDIR)$(pkgdatadir)/userHandbook ;\
	  cp -r images html $(DESTDIR)$(pkgdatadir)/userHandbook/. ;\
	fi;

uninstall-am :
	rm -rf $(DESTDIR)$(pkgdatadir)/userHandbook

all : 

check :
	$(XMLLINT) --valid -noout $(srcdir)/index.docbook

clean :
	rm -rf html pdf profile.dcf *~

html : html/index.html profile.dcf


html/index.html : $(DOCBOOK_SRC) $(srcdir)/../common/tulip-html.xsl $(srcdir)/../common/CatalogManager.properties
	$(JAVA) -cp "$(top_srcdir)/thirdparty/saxon/saxon653.jar:$(top_srcdir)/thirdparty/saxon/saxon.jar:$(srcdir)/../common:$(top_srcdir)/thirdparty/xml-commons/resolver.jar" \
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u \
	$(srcdir)/index.docbook  $(srcdir)/../common/tulip-html.xsl


pdf :   pdf/userManual.fo pdf/userManual.pdf

pdf/userManual.fo : $(DOCBOOK_SRC) $(srcdir)/../common/tulip-pdf.xsl $(srcdir)/../common/CatalogManager.properties
	$(JAVA) -cp "$(top_srcdir)/thirdparty/saxon/saxon653.jar:$(top_srcdir)/thirdparty/saxon/saxon.jar:$(srcdir)/../common:$(top_srcdir)/thirdparty/xml-commons/resolver.jar" \
	com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u -o pdf/userManual.fo \
	$(srcdir)/index.docbook  $(srcdir)/../common/tulip-pdf.xsl

pdf/userManual.pdf : pdf/userManual.fo
	$(PDFXMLTEX) --output-directory="pdf/" pdf/userManual.fo && $(PDFXMLTEX) --output-directory="pdf/" pdf/userManual.fo

profile.dcf : $(srcdir)/../common/tulip-profile.xsl $(DOCBOOK_SRC) $(srcdir)/../common/CatalogManager.properties
	$(JAVA) -cp "$(top_srcdir)/thirdparty/saxon/saxon653.jar:$(top_srcdir)/thirdparty/saxon/saxon.jar:$(srcdir)/../common:$(top_srcdir)/thirdparty/xml-commons/resolver.jar" \
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -o profile.dcf $(srcdir)/index.docbook  $(srcdir)/../common/tulip-profile.xsl install.path="userHandbook/html/"


