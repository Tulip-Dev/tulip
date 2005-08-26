EXTRA_DIST = index.docbook images/*.png

DOCBOOK_SRC = index.docbook functions.docbook graph_interface.docbook tutorials.docbook

install-data-am :
	if test -d html ; then \
		mkdir -p $(pkgdatadir)/userHandbook ;\
		cp -r images html $(pkgdatadir)/userHandbook/. ;\
	fi;

uninstall-am :
	rm -rf $(pkgdatadir)/userHandbook

all : html

check :
	$(XMLLINT) --valid -noout index.docbook

clean :
	rm -rf html pdf profile.dcf *~

html : html/index.html profile.dcf

html/index.html : $(DOCBOOK_SRC) ../common/tulip-html.xsl ../common/CatalogManager.properties
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u\
	index.docbook  ../common/tulip-html.xsl


pdf :   pdf/userManual.fo pdf/userManual.pdf

pdf/userManual.fo : $(DOCBOOK_SRC) ../common/tulip-pdf.xsl ../common/CatalogManager.properties
	$(JAVA) -cp "$(top_srcdir)/thirdparty/saxon/saxon653.jar:$(top_srcdir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u -o pdf/userManual.fo \
	index.docbook  ../common/tulip-pdf.xsl

pdf/userManual.pdf : pdf/userManual.fo
	$(PDFXMLTEX) --output-directory="pdf/" pdf/userManual.fo && $(PDFXMLTEX) --output-directory="pdf/" pdf/userManual.fo

profile.dcf : ../common/tulip-profile.xsl $(DOCBOOK_SRC) ../common/CatalogManager.properties
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader\
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -o profile.dcf index.docbook  ../common/tulip-profile.xsl install.path="userHandbook/html/"


