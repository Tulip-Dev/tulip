EXTRA_DIST = index.docbook images/*.png

install-data-am :
	mkdir -p $(pkgdatadir)/userHandbook
	cp -r images *.html $(pkgdatadir)/userHandbook

uninstall :
	rm -rf $(pkgdatadir)/userHandbook

all :

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
	-r org.apache.xml.resolver.tools.CatalogResolver -u\
	index.docbook  ../common/tulip-html.xsl


pdf :   userManual.fo userManual.pdf

userManual.fo : index.docbook ../common/tulip-pdf.xsl ../common/CatalogManager.properties
	$(JAVA) -cp "$(top_srcdir)/thirdparty/saxon/saxon653.jar:$(top_srcdir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet -x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u -o userManual.fo \
	index.docbook  ../common/tulip-pdf.xsl

userManual.pdf : userManual.fo
	pdfxmltex userManual.fo && pdfxmltex userManual.fo

profile.dcf : ../common/tulip-profile.xsl index.docbook ../common/CatalogManager.properties
	$(JAVA) -cp "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader\
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -o profile.dcf index.docbook  ../common/tulip-profile.xsl install.path="userHandbook/"


