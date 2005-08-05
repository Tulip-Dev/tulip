EXTRA_DIST = index.docbook \
	images/*.png

install-data-am :
	cp -r images $(pkgdatadir)/developerHandbook
	cp -r *.html $(pkgdatadir)/developerHandbook

uninstall :
	rm -rf $(pkgdatadir)/developerHandbook/images	
	rm -f $(pkgdatadir)/developerHandbook/*.html


all :

check :
	$(XMLLINT) --valid -noout index.docbook

clean :
	rm -f *~ *.html *.out *.log *.out *.fo *.aux *.dvi *.pdf profile.dcf

html : index.html profile.dcf

index.html : index.docbook ../common/tulip-html.xsl ../common/CatalogManager.properties
	$(JAVA) --classpath "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u\
	index.docbook  ../common/tulip-html.xsl


pdf :   devHandbook.fo devHandbook.pdf

devHandbook.fo : index.docbook ../common/tulip-pdf.xsl ../common/CatalogManager.properties
	$(JAVA) --classpath "$(top_srcdir)/thirdparty/saxon/saxon653.jar:$(top_srcdir)/thirdparty/saxon/saxon.jar:../common:/usr/share/java/xml-commons-resolver.jar" \
	com.icl.saxon.StyleSheet -r org.apache.xml.resolver.tools.CatalogResolver \
	-o  devHandbook.fo index.docbook  ../common/tulip-pdf.xsl

devHandbook.pdf : devHandbook.fo
	pdfxmltex devHandbook.fo && pdfxmltex devHandbook.fo

profile.dcf : ../common/tulip-profile.xsl index.docbook
	$(JAVA) --classpath "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar"\
	com.icl.saxon.StyleSheet -o profile.dcf index.docbook  ../common/tulip-profile.xsl path="developerHandbook/"



