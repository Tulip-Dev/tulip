EXTRA_DIST = index.docbook \
	images/*.png

install-data-am :
	if test -f index.html ; then \
		mkdir -p $(pkgdatadir)/developerHandbook ;\
		cp -r images $(pkgdatadir)/developerHandbook ;\
		cp -r *.html $(pkgdatadir)/developerHandbook ;\
	fi;

uninstall-am :
	rm -rf $(pkgdatadir)/developerHandbook

all : html

check :
	$(XMLLINT) --valid -noout index.docbook

clean :
	rm -f *~ *.html *.out *.log *.out *.fo *.aux *.dvi *.pdf profile.dcf

html : index.html profile.dcf

index.html : index.docbook ../common/tulip-html.xsl ../common/CatalogManager.properties
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u \
	index.docbook  ../common/tulip-html.xsl


pdf :   devHandbook.fo devHandbook.pdf

devHandbook.fo : index.docbook ../common/tulip-pdf.xsl ../common/CatalogManager.properties
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar" \
	com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader\
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u\
	-o  devHandbook.fo index.docbook  ../common/tulip-pdf.xsl

devHandbook.pdf : devHandbook.fo
	$(PDFXMLTEX) devHandbook.fo && $(PDFXMLTEX) devHandbook.fo


profile.dcf : ../common/tulip-profile.xsl index.docbook ../common/CatalogManager.properties
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader\
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u\
	-o profile.dcf index.docbook  ../common/tulip-profile.xsl install.path="developerHandbook/"

