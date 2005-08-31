EXTRA_DIST = index.docbook \
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
		mkdir -p $(pkgdatadir)/developerHandbook ;\
		cp -r images $(pkgdatadir)/developerHandbook ;\
		cp -r html $(pkgdatadir)/developerHandbook ;\
	fi;

uninstall-am :
	rm -rf $(pkgdatadir)/developerHandbook

all :

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
	-r org.apache.xml.resolver.tools.CatalogResolver -u \
	index.docbook  ../common/tulip-html.xsl

pdf :   pdf/devHandbook.fo pdf/devHandbook.pdf

pdf/devHandbook.fo : $(DOCBOOK_SRC) ../common/tulip-pdf.xsl ../common/CatalogManager.properties
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar" \
	com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader\
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u\
	-o pdf/devHandbook.fo index.docbook  ../common/tulip-pdf.xsl

pdf/devHandbook.pdf : pdf/devHandbook.fo
	$(PDFXMLTEX) --output-directory="pdf/" pdf/devHandbook.fo && \
	$(PDFXMLTEX) --output-directory="pdf/" pdf/devHandbook.fo


profile.dcf : ../common/tulip-profile.xsl $(DOCBOOK_SRC) ../common/CatalogManager.properties
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader\
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u\
	-o profile.dcf index.docbook  ../common/tulip-profile.xsl install.path="developerHandbook/html/"

